#include "Chunk.h"

#include <iostream>
#include <NBTField/NBTField.h>

#include "Block/TileEntities/TileEntity.h"
#include "Block/TileEntities/TileEntityManager.h"
#include "Block/BlockList.h"
#include "Entity/EntityPlayer.h"
#include "Logging/Logger.h"
#include "Network/OpcodeList.h"
#include "Util/types.h"
#include "Util/IntUtil.h"
#include "World/World.h"

namespace World
{

Chunk::Chunk(int x, int z, World* world) :
    posX(x), posZ(z), loaded(false)
    , cachePacket(Network::OP_CHUNK_DATA)
    , removeChunkPacket(Network::OP_CHUNK_DATA)
    , blockChangePacket(Network::OP_MULTI_BLOCK_CHANGE)
    , flagSectionExists(0)
    , flagSectionUseAdd(0)
    , inCache(false)
    , needSave(false)
    , countChange(0)
    , world(world)
    , selfTickCounter(0)
    , minHeight(0)
    , skylightNeedUpdate(false)
    , skylightColumnsToUpdate{false}
{
    posXx16 = x * 16;
    posZx16 = z * 16;
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
        datas[i] = nullptr;
    for (int i = 0; i < CHUNK_SURFACE; i++)
        heightMap[i] = 0;
    removeChunkPacket << posX << posZ << true << static_cast<unsigned short>(0) << static_cast<unsigned short>(0) << 0;
    ResetBlockChangePacket();
}

Chunk::~Chunk()
{
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
    {
        if (datas[i] != NULL)
        {
            if (datas[i]->addData != NULL)
                delete[] datas[i]->addData;
            delete datas[i];
        }
    }

    for (auto tileEntityItr : tileEntities)
    {
        delete tileEntityItr.second;
    }
    activeTileEntities.clear();
    updatedTileEntities.clear();
    tileEntities.clear();

}

void Chunk::Load()
{
    flagSectionExists = 0;
    flagSectionUseAdd = 0;

    NBT::TagCompound* nbtData = world->getChunkNbtData(posX, posZ);
    if (!loadFromNbtData(nbtData))
    {
        if (nbtData)
        {
            LOG_ERROR << "Data found but error occure while loading chunk: " << posX << " " << posZ << std::endl;
        }
        // Clear all
        {
            unsigned char* data = biomeData;
            unsigned int* heightdata = heightMap;
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
            chunkData->addData = nullptr;
            datas[0] = chunkData;
            flagSectionExists |= 1;
        }


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
        chunkData->addData = nullptr;
        datas[1] = chunkData;
        flagSectionExists |= (1 << 1);
    }

    ChunkData** chunkDataItr = datas;
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
    {
        if (*chunkDataItr)
        {
            ChunkData* chunkData = (*chunkDataItr);

            unsigned char* data = chunkData->blocks;//TODO: use add data for 4096 id
            for (int i = 0; i < CHUNK_BLOCK_COUNT; i++)
            {
                const Block::Block* block = Block::BlockList::getBlock(*data);
                if (block && block->NeedsRandomTick())
                {
                    chunkData->countRandomUpdate++;
                }
                data++;
            }
        }
        chunkDataItr++;
    }

    loaded = true;
    delete nbtData;
}

void Chunk::Save()
{
    needSave = false;
    NBT::TagCompound* tag = saveToNbtData();
    world->saveChunkNbtData(posX, posZ, tag);
}

void Chunk::UpdateTick()
{
    if (playerList.empty())
        return;
    if (!loaded)
        return;

    UpdateSkyLightIFN();

    // Random update tick
    i_block blockId;
    i_data blockData;
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
    {
        ChunkData* chunkData = datas[i];
        if (chunkData != nullptr)
        {
            if (chunkData->countRandomUpdate <= 0)
                continue;
            for (int count = 0; count < 3; count++)
            {
                int random = Util::FastGenRandomInt();
                unsigned int cellId = (random >> 2) & 0xfff;
                i_small_coord blockX = cellId & 0xf;
                i_small_coord blockZ = ((cellId >> 4) & 0xf);
                i_height blockY = (i << 4) + ((cellId >> 8) & 0xf);

                blockId = getBlockAt(blockX, blockY, blockZ);
                if (blockId > 0)
                {
                    const Block::Block* block = Block::BlockList::getBlock(blockId);
                    if (block != nullptr)
                    {
                        if (block->NeedsRandomTick())
                        {
                            blockData = getDataAt(blockX, blockY, blockZ);
                            block->UpdateTick(world, posXx16 + (cellId & 0xf), (i << 4) + ((cellId >> 8) & 0xf), posZx16 + ((cellId >> 4) & 0xf), blockData);
                        }
                    }
                }
            }
        }
    }

    while (!toUpdateBlockList.empty())
    {
        const UpdateBlockData& updateData = toUpdateBlockList.top();
        if (updateData.updateTick > selfTickCounter)
            break;
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
        toUpdateBlockList.pop();
    }

    for (auto tileEntity: activeTileEntities)
    {
        tileEntity->UpdateTick();
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

void Chunk::GetTileEntityPacket(Network::NetworkPacket& packet) const
{
    for (auto tileEntityItr : tileEntities)
    {
        if (tileEntityItr.second->HasNetworkData())
            tileEntityItr.second->GetDataPacket(packet);
    }
}


void Chunk::ChangeBlockNoEventNoTileEntityChange_DoNotUseExceptIfYouKnowWhatYouDo(i_small_coord x, i_height y, i_small_coord z, i_block blockID, i_data blockData)
{
    inCache = false;
    needSave = true;

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

void Chunk::ChangeBlock(i_small_coord x, i_height y, i_small_coord z, i_block blockID, i_data blockData)
{
    inCache = false;
    needSave = true;

    i_block previousBlockId = getBlockAt(x, y, z);
    i_data previousBlockkData = getDataAt(x, y, z);

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

    const Block::Block* previousBlock = Block::BlockList::getBlock(previousBlockId);
    if (previousBlock)
    {
        previousBlock->Destroy(world, x + posXx16, y, z + posZx16, previousBlockkData);
        if (previousBlock->UseTileEntity())
            RemoveTileEntity(x, y, z);
        if (previousBlock->NeedsRandomTick())
        {
            ChunkData* data = getOrCreateData(y >> 4);
            data->countRandomUpdate--;
        }
    }

    const Block::Block* block = Block::BlockList::getBlock(blockID);
    if (block)
    {
        block->OnBlockAddedInWorld(world, x + posXx16, y, z + posZx16, blockData);
        if (block->UseTileEntity())
        {
            Block::TileEntity* tileEntity = block->CreateNewTileEntity(world, x + posXx16, y, z + posZx16);
            SetTileEntity(tileEntity ,x, y, z);
        }
        if (block->NeedsRandomTick())
        {
            ChunkData* data = getOrCreateData(y >> 4);
            data->countRandomUpdate++;
        }
    }
}
void Chunk::ChangeData(i_small_coord x, i_height y, i_small_coord z, i_data blockData)
{
    inCache = false;
    needSave = true;

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
    player->Send(removeChunkPacket);
    playerList.erase(player);
}

void Chunk::SendUpdate()
{
    if (countChange > 0)
    {
        blockChangePacket.UpdateAt(9, countChange);
        blockChangePacket.UpdateAt(11, (int)(countChange * 4));
        for (EntityPlayer* plr : playerList)
        {
            plr->Send(blockChangePacket);
        }
        ResetBlockChangePacket();
    }

    if (!updatedTileEntities.empty())
    {
        tileEntityUpdatePacket.Clear();
        for (Block::TileEntity* tileEntity: updatedTileEntities)
        {
            tileEntity->GetDataPacket(tileEntityUpdatePacket);
        }
        if (tileEntityUpdatePacket.getPacketSize() > 0)
        {
            for (EntityPlayer* plr : playerList)
            {
                plr->Send(tileEntityUpdatePacket);
            }
        }
        updatedTileEntities.clear();
    }
}


void Chunk::SetTileEntity(Block::TileEntity* tileEntity, i_small_coord x, i_height y, i_small_coord z)
{
    tileEntities[TILEENTITY_KEY(x, y, z)] = tileEntity;
    MarkForNetworkUpdateTileEntity(x, y, z);
    if (tileEntity->NeedUpdate())
        activeTileEntities.insert(tileEntity);
}


void Chunk::MarkForNetworkUpdateTileEntity(i_small_coord x, i_height y, i_small_coord z)
{
    Block::TileEntity* tileEntity = GetTileEntity(x, y, z);
    if (tileEntity != nullptr)
    {
        if (tileEntity->HasNetworkData())
            updatedTileEntities.insert(tileEntity);
    }
}


Block::TileEntity* Chunk::GetTileEntity(i_small_coord x, i_height y, i_small_coord z) const
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
        updatedTileEntities.erase(tileEntity);
        activeTileEntities.erase(tileEntity);
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

    if (!world->isChunksExistInRange(posXx16 + 8, posZx16 + 8, 16))
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
    needSave = true;

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

i_lightvalue Chunk::getRealBlockLightValue(i_small_coord x, i_height y, i_small_coord z, i_lightvalue sunReduceValue) const
{
    i_lightvalue skyLight = getSkyLightAt(x, y, z);
    skyLight = std::max(0, skyLight - sunReduceValue);
    i_lightvalue blockLight = getBlockLightAt(x, y, z);
    blockLight = std::max(skyLight, blockLight);
    return blockLight;
}

i_lightopacity Chunk::getBlockLightOpacity(i_small_coord x, i_height y, i_small_coord z)
{
    i_block blockId = getBlockAt(x, y, z);
    const Block::Block* block = Block::BlockList::getBlock(blockId);
    if (block)
    {
        return block->GetLightOpacity();
    }
    return 0;
}

i_height Chunk::getMinHeight() const
{
    return minHeight;
}

bool Chunk::loadFromNbtData(NBT::TagCompound* nbtData)
{
    if (nbtData)
    {
        NBT::TagCompound* level = nbtData->GetTagAs<NBT::TagCompound>("Level");
        if (!level)
            return false;

        NBT::TagByteArray* biomesArray = level->GetTagAs<NBT::TagByteArray>("Biomes");
        if (biomesArray != nullptr)
        {
            if (biomesArray->GetSize() != CHUNK_SURFACE)
                return false;

            memcpy(biomeData, biomesArray->GetData(), CHUNK_SURFACE);
        }


        NBT::TagIntArray* heightMapArray = level->GetTagAs<NBT::TagIntArray>("HeightMap");
        if (!heightMapArray)
            return false;
        if (heightMapArray->GetSize() != CHUNK_SURFACE)
            return false;
        for (int i = 0; i < CHUNK_SURFACE; i++)
            heightMap[i] = heightMapArray->GetData()[i];


        NBT::TagList* sections = level->GetTagAs<NBT::TagList>("Sections");
        if (!sections)
            return false;

        const std::vector<NBT::Tag *>& sectionsList = sections->GetTagList();
        for (NBT::Tag* tag : sectionsList)
        {
            NBT::TagCompound* chunkSection = dynamic_cast<NBT::TagCompound*>(tag);
            if (!chunkSection)
                return false;

            NBT::TagByte* yPos = chunkSection->GetTagAs<NBT::TagByte>("Y");
            if (!yPos)
                return false;
            int chunkSectionId = yPos->GetValue();
            if (chunkSectionId >= CHUNK_DATA_COUNT || chunkSectionId < 0)
                return false;

            ChunkData* chunkData = getOrCreateData(chunkSectionId);

            NBT::TagByteArray* metadataArray = chunkSection->GetTagAs<NBT::TagByteArray>("Data");
            if (!metadataArray)
                return false;
            if (metadataArray->GetSize() != CHUNK_BLOCK_NIBBLE_SIZE)
                return false;
            memcpy(chunkData->metadata, metadataArray->GetData(), CHUNK_BLOCK_NIBBLE_SIZE);

            NBT::TagByteArray* skyLightArray = chunkSection->GetTagAs<NBT::TagByteArray>("SkyLight");
            if (!skyLightArray)
                return false;
            if (skyLightArray->GetSize() != CHUNK_BLOCK_NIBBLE_SIZE)
                return false;
            memcpy(chunkData->skyLight, skyLightArray->GetData(), CHUNK_BLOCK_NIBBLE_SIZE);


            NBT::TagByteArray* blockLightArray = chunkSection->GetTagAs<NBT::TagByteArray>("BlockLight");
            if (!blockLightArray)
                return false;
            if (blockLightArray->GetSize() != CHUNK_BLOCK_NIBBLE_SIZE)
                return false;
            memcpy(chunkData->blocklight, blockLightArray->GetData(), CHUNK_BLOCK_NIBBLE_SIZE);

            NBT::TagByteArray* blocksArray = chunkSection->GetTagAs<NBT::TagByteArray>("Blocks");
            if (!blocksArray)
                return false;
            if (blocksArray->GetSize() != CHUNK_BLOCK_COUNT)
                return false;
            memcpy(chunkData->blocks, blocksArray->GetData(), CHUNK_BLOCK_COUNT);

            NBT::TagByteArray* addMetadataArray = chunkSection->GetTagAs<NBT::TagByteArray>("Add");
            if (addMetadataArray != nullptr)
            {
                if (addMetadataArray->GetSize() != CHUNK_BLOCK_NIBBLE_SIZE)
                    return false;
                if (chunkData->addData == nullptr)
                {
                    chunkData->addData = new unsigned char[CHUNK_BLOCK_NIBBLE_SIZE];
                    chunkData->clearAddData();
                    flagSectionUseAdd |= (1 << chunkSectionId);
                }
                memcpy(chunkData->addData, addMetadataArray->GetData(), CHUNK_BLOCK_NIBBLE_SIZE);
            }
        }

        NBT::TagList* tileEntities = level->GetTagAs<NBT::TagList>("TileEntities");
        if (tileEntities)
        {
            const std::vector<NBT::Tag *>& tileEntitiesList = tileEntities->GetTagList();
            for (NBT::Tag* tag : tileEntitiesList)
            {
                NBT::TagCompound* tileEntityData = dynamic_cast<NBT::TagCompound*>(tag);
                if (!tileEntityData)
                    continue;

                NBT::TagString* tileEntityId = tileEntityData->GetTagAs<NBT::TagString>("id");
                if (!tileEntityId)
                    continue;

                NBT::TagInt* tileEntityX = tileEntityData->GetTagAs<NBT::TagInt>("x");
                if (!tileEntityX)
                    continue;
                NBT::TagInt* tileEntityY = tileEntityData->GetTagAs<NBT::TagInt>("y");
                if (!tileEntityY)
                    continue;
                NBT::TagInt* tileEntityZ = tileEntityData->GetTagAs<NBT::TagInt>("z");
                if (!tileEntityZ)
                    continue;

                int x = tileEntityX->GetValue();
                i_height y = tileEntityY->GetValue();
                int z = tileEntityZ->GetValue();
                Block::TileEntity* tileEntity = Block::TileEntityManager::Instance().GetNewTileEntityByName(tileEntityId->GetValue(), world, x, y, z);
                if (tileEntity)
                {
                    tileEntity->Load(tileEntityData);
                    SetTileEntity(tileEntity, x & 0xf, y, z & 0xf);
                }
            }
            return true;
        }

        NBT::TagList* tileTicks = level->GetTagAs<NBT::TagList>("TileTicks");
        if (tileTicks)
        {
            const std::vector<NBT::Tag *>& toUpdateBlock = tileTicks->GetTagList();
            for (NBT::Tag* tag : toUpdateBlock)
            {
                NBT::TagCompound* tileTickData = dynamic_cast<NBT::TagCompound*>(tag);
                if (!tileTickData)
                    continue;
                NBT::TagInt* tileTickX = tileTickData->GetTagAs<NBT::TagInt>("x");
                if (!tileTickX)
                    continue;
                int x = tileTickX->GetValue();

                NBT::TagInt* tileTickY = tileTickData->GetTagAs<NBT::TagInt>("y");
                if (!tileTickY)
                    continue;
                int y = tileTickY->GetValue();

                NBT::TagInt* tileTickZ = tileTickData->GetTagAs<NBT::TagInt>("z");
                if (!tileTickZ)
                    continue;
                int z = tileTickZ->GetValue();

                NBT::TagInt* tileTickI = tileTickData->GetTagAs<NBT::TagInt>("i");
                if (!tileTickI)
                    continue;
                int i = tileTickI->GetValue();

                NBT::TagInt* tileTickT = tileTickData->GetTagAs<NBT::TagInt>("t");
                if (!tileTickT)
                    continue;
                int t = tileTickT->GetValue();

                /*Unused yet, optionnal for old map
                 * NBT::TagInt* tileTickP = tileTickData->GetTagAs<NBT::TagInt>("p");
                if (!tileTickP)
                    continue;*/
                MarkForUpdate(x & 0xf, y, z & 0xf, i, t);
            }
            return true;
        }
    }
    return false;
}


NBT::TagCompound* Chunk::saveToNbtData() const
{
    using namespace NBT;
    TagCompound* tagLevel = new TagCompound("Level");

    tagLevel->AddByteArray("Biomes", reinterpret_cast<const char*>(biomeData), CHUNK_SURFACE);
    tagLevel->AddIntArray("HeightMap", reinterpret_cast<const int*>(heightMap), CHUNK_SURFACE);

    // Saving all sub chunk data (16x16x16)
    {
        TagList* tagSections = new TagList("Sections", TagType::TAG_COMPOUND);

        for (int chunkSectionId = 0; chunkSectionId < CHUNK_DATA_COUNT; chunkSectionId++)
        {
            ChunkData* chunkData =  datas[chunkSectionId];
            if (chunkData != NULL)
            {
                TagCompound* sectionData = new TagCompound();

                sectionData->AddByte("Y", chunkSectionId);
                sectionData->AddByteArray("Data", reinterpret_cast<char*>(chunkData->metadata), CHUNK_BLOCK_NIBBLE_SIZE);
                sectionData->AddByteArray("SkyLight", reinterpret_cast<char*>(chunkData->skyLight), CHUNK_BLOCK_NIBBLE_SIZE);
                sectionData->AddByteArray("BlockLight", reinterpret_cast<char*>(chunkData->blocklight), CHUNK_BLOCK_NIBBLE_SIZE);
                sectionData->AddByteArray("Blocks", reinterpret_cast<char*>(chunkData->blocks), CHUNK_BLOCK_COUNT);
                if (chunkData->addData)
                    sectionData->AddByteArray("Add", reinterpret_cast<char*>(chunkData->addData), CHUNK_BLOCK_NIBBLE_SIZE);

                tagSections->AddTag(sectionData);
            }
        }

        tagLevel->AddTag(tagSections);
    }

    // Saving tile entities
    TagList* tagTileEntities = new TagList("TileEntities", TagType::TAG_COMPOUND);
    for(auto tileEntityItr : tileEntities)
    {
        const Block::TileEntity* tileEntity = tileEntityItr.second;
        TagCompound* tagTileEntity = new TagCompound();

        tagTileEntity->AddString("id", tileEntity->GetName());
        tagTileEntity->AddInt("x", tileEntity->GetBlockX());
        tagTileEntity->AddInt("y", tileEntity->GetBlockY());
        tagTileEntity->AddInt("z", tileEntity->GetBlockZ());

        tileEntity->Save(tagTileEntity);

        tagTileEntities->AddTag(tagTileEntity);
    }
    tagLevel->AddTag(tagTileEntities);

    TagList* tagUpdateBlockList = new TagList("TileTicks", TagType::TAG_COMPOUND);
    for (auto updateBlockItr : toUpdateBlockList)
    {
        UpdateBlockData updateData = updateBlockItr;

        TagCompound* tagUpdateData = new TagCompound();

        tagUpdateData->AddInt("x", updateData.coord.coord.x + posXx16);
        tagUpdateData->AddInt("y", updateData.coord.coord.y + updateData.coord.coord.chunkDataY * 16);
        tagUpdateData->AddInt("z", updateData.coord.coord.z + posZx16);
        tagUpdateData->AddInt("i", updateData.blockId);
        tagUpdateData->AddInt("t", updateData.updateTick - selfTickCounter);

        tagUpdateBlockList->AddTag(tagUpdateData);
    }
    tagLevel->AddTag(tagUpdateBlockList);

    TagCompound* tagLData = new TagCompound();
    tagLData->AddTag(tagLevel);
    return tagLData;
}

} /* namespace World */
