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

namespace World
{

Chunk::Chunk(int x, int z, World* world) :
    posX(x), posZ(z), loaded(false)
    , cachePacket(Network::OP_CHUNK_DATA), blockChangePacket(Network::OP_MULTI_BLOCK_CHANGE)
    , flagSectionExists(0), flagSectionUseAdd(0), inCache(false), countChange(0), world(world)
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
        for (int i = 0; i < CHUNK_SURFACE; i++)
        {
            *data = 0;
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
            *data = 0xff;
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
            *data = 0xff;
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
                Block::Block* block = Block::BlockList::getBlock(blockId);
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
            Block::Block* block = Block::BlockList::getBlock(blockId);
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
    Block::Block* previousBlock = Block::BlockList::getBlock(previousBlockId);
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

} /* namespace Network */
