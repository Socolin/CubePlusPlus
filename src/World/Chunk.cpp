/*
 * Chunk.cpp
 *
 *  Created on: 5 déc. 2012
 *      Author: bertrand
 */

#include "Chunk.h"

#include <iostream>

#include "Block/TileEntities/TileEntity.h"
#include "Block/BlockList.h"
#include "Entity/EntityPlayer.h"
#include "Network/OpcodeList.h"
#include "Util/types.h"
#include "Util/IntUtil.h"
#include "World/World.h"

namespace World
{

Chunk::Chunk(int x, int z, World* world) :
    posX(x), posZ(z), loaded(false)
    , cachePacket(Network::OP_CHUNK_DATA), blockChangePacket(Network::OP_MULTI_BLOCK_CHANGE)
    , flagSectionExists(0), flagSectionUseAdd(0), inCache(false), countChange(0), world(world)
    , minHeight(0), skylightNeedUpdate(false), skylightColumnsToUpdate{false}
{
    posXx16 = x * 16;
    posZx16 = z * 16;
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
        datas[i] = NULL;
    ResetBlockChangePacket();
}

Chunk::~Chunk()
{
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
    {
        if (datas[i] != NULL)
        {
            if (datas[i]->addData != NULL)
                delete datas[i]->addData;
            delete datas[i];
        }
    }
}

void Chunk::Load()
{
    flagSectionExists = 0;
    flagSectionUseAdd = 0;
    {
        unsigned char* data = biomeData;
        unsigned char* heightdata = heightMap;
        for (int i = 0; i < CHUNK_SURFACE; i++)
        {
            *data = 0;
            *heightdata = 32;
            heightdata++;
            data++;
        }
    }
    {
        ChunkData* chunkData = new ChunkData();
        unsigned char* data = chunkData->blocks;
        for (int i = 0; i < CHUNK_BLOCK_COUNT; i++)
        {
            *data = 35;
            data++;
        }
        data = chunkData->metadata;
        for (int i = 0; i < CHUNK_BLOCK_NIBBLE_SIZE; i++)
        {
            *data = (i * 2 )% 16;
            *data |= (((i * 2) + 1) % 16) << 4;

            data++;
        }
        data = chunkData->blocklight;
        for (int i = 0; i < CHUNK_BLOCK_NIBBLE_SIZE; i++)
        {
            *data = 0;
            data++;
        }
        data = chunkData->skyLight;
        for (int i = 0; i < CHUNK_BLOCK_NIBBLE_SIZE; i++)
        {
            *data = 0x0;
            data++;
        }
        chunkData->addData = NULL;
        datas[0] = chunkData;
        flagSectionExists |= 1;
    }

    {
        ChunkData* chunkData = new ChunkData();
        unsigned char* data = chunkData->blocks;
        for (int i = 0; i < CHUNK_BLOCK_COUNT; i++)
        {
            *data = 35;
            data++;
        }
        data = chunkData->metadata;
        for (int i = 0; i < CHUNK_BLOCK_NIBBLE_SIZE; i++)
        {
            *data = (i )% 16;

            data++;
        }
        data = chunkData->blocklight;
        for (int i = 0; i < CHUNK_BLOCK_NIBBLE_SIZE; i++)
        {
            *data = 0;
            data++;
        }
        data = chunkData->skyLight;
        for (int i = 0; i < CHUNK_BLOCK_NIBBLE_SIZE; i++)
        {
            *data = 0;
            data++;
        }
        chunkData->addData = NULL;
        datas[1] = chunkData;
        flagSectionExists |= (1 << 1);
    }
    loaded = true;
}

void Chunk::UpdateTick()
{
    if (!loaded)
        return;

    ChunkData** chunkDataItr = datas;

    UpdateSkyLightIFN();

    // Random update tick
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
    {
        if (*chunkDataItr)
        {
            ChunkData* chunkData = (*chunkDataItr);
            int random = Util::FastGenRandomInt();
            unsigned int cellId = random & 0xfff;
            i_block blockId;
            i_data blockData;
            if (chunkData->addData != NULL)
            {
                blockId = chunkData->blocks[cellId] | (chunkData->addData[cellId << 1] & (0xf << ((cellId & 0x1) << 2)));
            }
            else
            {
                blockId = chunkData->blocks[cellId];
            }
            if (blockId > 0)
            {
                const Block::Block* block = Block::BlockList::getBlock(blockId);
                if (block)
                {
                    if ((cellId & 0x1) == 0)
                    {
                        blockData = chunkData->metadata[cellId >> 1] & 0xf;
                    }
                    else
                    {
                        blockData = (chunkData->metadata[cellId >> 1] & 0xf0) >> 4;
                    }
                    if (block->isNeedsRandomTick())
                    {
                        block->UpdateTick(world, posXx16 + (cellId & 0xf), (i << 4) + ((cellId >> 8) & 0xf), posZx16 + ((cellId >> 4) & 0xf), blockData);
                    }
                }
            }

        }
        chunkDataItr++;
    }

    while (!toUpdateBlockList.empty())
    {
        const UpdateBlockData& updateData = toUpdateBlockList.top();
        if (updateData.updateTick > selfTickCounter)
            break;
        toUpdateBlockList.pop();
        unsigned int cellId = updateData.coord.cellId & 0xfff;
        i_block blockId;
        i_data blockData;
        ChunkData* chunkData = datas[updateData.coord.coord.chunkDataY];
        if (chunkData->addData != NULL)
        {
            blockId = chunkData->blocks[cellId] | (chunkData->addData[cellId << 1] & (0xf << ((cellId & 0x1) << 2)));
        }
        else
        {
            blockId = chunkData->blocks[cellId];
        }
        if (blockId == updateData.blockId && blockId > 0)
        {
            const Block::Block* block = Block::BlockList::getBlock(blockId);
            if (block)
            {
                if ((cellId & 0x1) == 0)
                {
                    blockData = chunkData->metadata[cellId >> 1] & 0xf;
                }
                else
                {
                    blockData = (chunkData->metadata[cellId >> 1] & 0xf0) >> 4;
                }
                block->UpdateTick(world,
                        posXx16 + updateData.coord.coord.x,
                        (updateData.coord.coord.chunkDataY << 4) + updateData.coord.coord.y,
                        posZx16 + updateData.coord.coord.z, blockData);
            }
        }
    }

    selfTickCounter++;
}

void Chunk::Unload()
{
}

const Network::NetworkPacket& Chunk::GetPacket()
{
    if (!inCache)
        GeneratePacket();
    return cachePacket;
}

void Chunk::ChangeBlock(i_small_coord x, i_height y, i_small_coord z, i_block blockID, i_data blockData)
{
    inCache = false;
    i_block previousBlockId = getBlockAt(x, y, z);
    const Block::Block* previousBlock = Block::BlockList::getBlock(previousBlockId);
    if (previousBlock && previousBlock->UseTileEntity())
    {
        RemoveTileEntity(x, y, z);
    }
    SetBlockAt(x, y, z, blockID);
    SetDataAt(x, y, z, blockData);
    unsigned int dataChange = 0;
    dataChange |= blockData       & 0x0000000f;
    dataChange |= (blockID << 4)  & 0x0000fff0;
    dataChange |= ((int)y << 16)  & 0x00ff0000;
    dataChange |= ((z & 0xf) << 24)       & 0x0f000000;
    dataChange |= ((x & 0xf) << 28)       & 0xf0000000;
    blockChangePacket << dataChange;
    countChange++;
}
void Chunk::ChangeData(i_small_coord x, i_height y, i_small_coord z, i_data blockData)
{
    inCache = false;
    i_block blockID = getBlockAt(x, y, z);
    SetDataAt(x, y, z, blockData);
    unsigned int dataChange = 0;
    dataChange |= blockData       & 0x0000000f;
    dataChange |= (blockID << 4)  & 0x0000fff0;
    dataChange |= ((int)y << 16)  & 0x00ff0000;
    dataChange |= ((z & 0xf) << 24)       & 0x0f000000;
    dataChange |= ((x & 0xf) << 28)       & 0xf0000000;
    blockChangePacket << dataChange;
    countChange++;
}
void Chunk::GeneratePacket()
{
    cachePacket.Reset();
    cachePacket << posX << posZ << true << flagSectionExists << flagSectionUseAdd;
    cachePacket.startWriteCompressedData();
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
    {
        ChunkData* chunkData =  datas[i];
        if (chunkData != NULL)
            cachePacket.appendCompress((char*)chunkData->blocks,CHUNK_BLOCK_COUNT);
    }
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
    {
        ChunkData* chunkData =  datas[i];
        if (chunkData != NULL)
            cachePacket.appendCompress((char*)chunkData->metadata,CHUNK_BLOCK_NIBBLE_SIZE);
    }
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
    {
        ChunkData* chunkData =  datas[i];
        if (chunkData != NULL)
            cachePacket.appendCompress((char*)chunkData->blocklight,CHUNK_BLOCK_NIBBLE_SIZE);
    }
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
    {
        ChunkData* chunkData =  datas[i];
        if (chunkData != NULL)
            cachePacket.appendCompress((char*)chunkData->skyLight,CHUNK_BLOCK_NIBBLE_SIZE);
    }
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
    {
        ChunkData* chunkData =  datas[i];
        if (chunkData != NULL && chunkData->addData != NULL)
            cachePacket.appendCompress((char*)chunkData->addData,CHUNK_BLOCK_NIBBLE_SIZE);
    }
    cachePacket.appendCompress((char*)biomeData,CHUNK_SURFACE);

    //cachePacket.dump();
    cachePacket.endWriteCompressedData();
    inCache = true;
}

void Chunk::AddPlayer(EntityPlayer* player)
{
    playerList.insert(player);
}

void Chunk::RemovePlayer(EntityPlayer* player)
{
    playerList.erase(player);
}

void Chunk::SendUpdate()
{
    if (countChange > 0)
    {
        blockChangePacket.UpdateAt(9, countChange);
        blockChangePacket.UpdateAt(11, (int)(countChange * 4));
        blockChangePacket.dump();
        for (EntityPlayer* plr : playerList)
        {
            plr->Send(blockChangePacket);
        }
        ResetBlockChangePacket();
    }
}


void Chunk::SetTileEntity(Block::TileEntity* tileEntity, i_small_coord x, i_height y, i_small_coord z)
{
    tileEntities[TILEENTITY_KEY(x, y, z)] = tileEntity;
}

Block::TileEntity* Chunk::GetTileEntity(i_small_coord x, i_height y, i_small_coord z)
{
    auto it = tileEntities.find(TILEENTITY_KEY(x, y, z));
    if (it == tileEntities.end())
    {
        return nullptr;
    }
    return it->second;
}

void Chunk::RemoveTileEntity(i_small_coord x, i_height y, i_small_coord z)
{
    auto it = tileEntities.find(TILEENTITY_KEY(x, y, z));
    if (it != tileEntities.end())
    {
        Block::TileEntity* tileEntity = it->second;
        tileEntities.erase(it);
        delete tileEntity;
    }
}

void Chunk::updateHeightMapAndSkyLight(i_small_coord x, i_height y, i_small_coord z)
{
    i_height terrainHeight = heightMap[z << 4 | x];
    i_height yLimit = terrainHeight;

    if (y > terrainHeight)
    {
        yLimit = y;
    }

    while (yLimit > 0 && getBlockLightOpacity(x, yLimit - 1, z) == 0)
    {
        --yLimit;
    }

    if (yLimit != terrainHeight)
    {
        int worldX = x + posXx16;
        int worldZ = z + posZx16;
        world->updateSkylightOnColumnt(worldX, worldZ, yLimit, terrainHeight);
        heightMap[z << 4 | x] = yLimit;


        {
            ChunkData* chunkData = nullptr;

            if (yLimit < terrainHeight)
            {
                for (i_height worldY = yLimit; worldY < terrainHeight; ++worldY)
                {
                    chunkData = datas[worldY >> 4];

                    if (chunkData != nullptr)
                    {
                        // TODO: optimize, direct write in chunkData->skylight
                        setSkyLightAt(x, worldY, z, 15);
                    }
                }
            }
            else
            {
                for (i_height worldY = terrainHeight; worldY < yLimit; ++worldY)
                {
                    chunkData = datas[worldY >> 4];

                    if (chunkData != nullptr)
                    {
                        // TODO: optimize, direct write in chunkData->skylight
                        setSkyLightAt(x, worldY, z, 0);
                    }
                }
            }
        }

        i_lightvalue blockLight = 15;
        i_lightopacity blockOpacity = 0;

        while (yLimit > 0 && blockLight > 0)
        {
            --yLimit;
            blockOpacity = getBlockLightOpacity(x, yLimit, z);
            blockOpacity = std::max(i_lightopacity(1), blockOpacity);

            if (blockLight > 0)
                blockLight -= blockOpacity;

            ChunkData* chunkData = datas[yLimit >> 4];

            if (chunkData != nullptr)
            {
                // TODO: optimize, direct write in chunkData->skylight
                setSkyLightAt(x, yLimit, z, blockLight);
            }
        }

        i_height currentHeightMap = heightMap[z << 4 | x];
        blockOpacity = terrainHeight;
        int maxY = currentHeightMap;

        if (currentHeightMap < terrainHeight)
        {
            blockOpacity = currentHeightMap;
            maxY = terrainHeight;
        }

        if (currentHeightMap < minHeight)
        {
            minHeight = currentHeightMap;
        }

        world->updateSkylightOnColumnt(worldX - 1, worldZ, blockOpacity, maxY);
        world->updateSkylightOnColumnt(worldX + 1, worldZ, blockOpacity, maxY);
        world->updateSkylightOnColumnt(worldX, worldZ - 1, blockOpacity, maxY);
        world->updateSkylightOnColumnt(worldX, worldZ + 1, blockOpacity, maxY);
        world->updateSkylightOnColumnt(worldX, worldZ, blockOpacity, maxY);
    }
}

void Chunk::MarkForUpdate(i_small_coord x, i_height y, i_small_coord z, i_block blockId, unsigned int tickWait)
{
    assert(tickWait > 0);
    UpdateBlockData updateData;
    updateData.coord.coord.x = x;
    updateData.coord.coord.z = z;
    updateData.coord.coord.y = y & 0xff;
    updateData.coord.coord.chunkDataY = (y >> 4) & 0xff;
    updateData.blockId = blockId;
    updateData.updateTick = tickWait + selfTickCounter;
    toUpdateBlockList.push(updateData);
}

void Chunk::ResetBlockChangePacket()
{
    blockChangePacket.Reset();
    blockChangePacket << posX << posZ << (short)0 << (int)0;
    countChange = 0;
}

void Chunk::UpdateSkyLightIFN()
{
    if (!skylightNeedUpdate)
        return;

    if (!world->isChunksExistInRange(posXx16 + 8, 0,  posZx16 + 8, 16))
        return;

    for (int xInChunk = 0; xInChunk < 16; ++xInChunk)
    {
        for (int zInChunk = 0; zInChunk < 16; ++zInChunk)
        {
            if (skylightColumnsToUpdate[xInChunk + zInChunk * 16])
            {
                skylightColumnsToUpdate[xInChunk + zInChunk * 16] = false;
                int terrainHeight = getHeightMapAt(xInChunk, zInChunk);
                int x = posXx16 + xInChunk;
                int z = posZx16 + zInChunk;
                i_height westHeightMap;
                i_height westBlockMinHeight = world->getMinHeightAndHeightMapAt(x - 1, z, westHeightMap);
                i_height eastHeightMap;
                i_height eastBlockMinHeight = world->getMinHeightAndHeightMapAt(x + 1, z, eastHeightMap);
                i_height northHeightMap;
                i_height northBlockMinHeight = world->getMinHeightAndHeightMapAt(x, z - 1, northHeightMap);
                i_height southHeightMap;
                i_height southBlockMinHeight = world->getMinHeightAndHeightMapAt(x, z + 1, southHeightMap);

                if (eastBlockMinHeight < westBlockMinHeight)
                {
                    westBlockMinHeight = eastBlockMinHeight;
                }

                if (northBlockMinHeight < westBlockMinHeight)
                {
                    westBlockMinHeight = northBlockMinHeight;
                }

                if (southBlockMinHeight < westBlockMinHeight)
                {
                    westBlockMinHeight = southBlockMinHeight;
                }

                world->updateSkylightOnColumnt(x, z, westBlockMinHeight, terrainHeight);
                world->updateSkylightOnColumnt(x - 1, z, terrainHeight, westHeightMap);
                world->updateSkylightOnColumnt(x + 1, z, terrainHeight, eastHeightMap);
                world->updateSkylightOnColumnt(x, z - 1, terrainHeight, northHeightMap);
                world->updateSkylightOnColumnt(x, z + 1, terrainHeight, westHeightMap);
            }
        }
    }

    skylightNeedUpdate = false;
}

void Chunk::propagateSkylightOcclusion(i_small_coord x, i_small_coord z)
{
    skylightColumnsToUpdate[x + z * 16] = true;
    skylightNeedUpdate = true;
}

void Chunk::GenerateSkyLight()
{
    i_height maxY = getFirstAvaibleChunkDataPositionFromTop() + 16 - 1;
    minHeight = 255;

    for (i_small_coord x = 0; x < 16; ++x)
    {
        i_small_coord z = 0;

        while (z < 16)
        {
            i_height y = maxY;

            while (true)
            {
                if (y > 0)
                {
                    if (getBlockLightOpacity(x, y - 1, z) == 0)
                    {
                        --y;
                        continue;
                    }

                    heightMap[z << 4 | x] = y;

                    if (y < minHeight)
                    {
                        minHeight = y;
                    }
                }

                y = 15;
                int blockY = maxY;

                do
                {
                    y -= getBlockLightOpacity(x, blockY, z);

                    if (y > 0)
                    {
                        ChunkData* chunkData = datas[blockY >> 4];

                        if (chunkData != nullptr)
                        {
                            // TODO optimize using chunkData->skyLight
                            setSkyLightAt(x, blockY, z, y);
                        }
                    }

                    --blockY;
                } while (blockY > 0 && y > 0);

                ++z;
                break;
            }
        }
    }

    inCache = false;

    for (i_small_coord x = 0; x < 16; ++x)
    {
        for (i_small_coord z = 0; z < 16; ++z)
        {
            propagateSkylightOcclusion(x, z);
        }
    }
}

i_height Chunk::getFirstAvaibleChunkDataPositionFromTop()
{
    i_height i = CHUNK_DATA_COUNT - 1;
    while (datas[i] == nullptr)
        i--;
    return i_height(i * 16);
}

i_lightopacity Chunk::getBlockLightOpacity(i_small_coord x, i_height y, i_small_coord z)
{
    i_block blockId = getBlockAt(x, y, z);
    const Block::Block* block = Block::BlockList::getBlock(blockId);
    if (block)
    {
        return block->getLightOpacity();
    }
    return 0;
}

i_height Chunk::getMinHeight() const
{
    return minHeight;
}

} /* namespace World */
