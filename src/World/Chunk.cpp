/*
 * Chunk.cpp
 *
 *  Created on: 5 déc. 2012
 *      Author: bertrand
 */

#include "Chunk.h"

#include <iostream>

#include "Entity/EntityPlayer.h"
#include "Network/OpcodeList.h"

namespace World
{

Chunk::Chunk(int x, int y) :
    posX(x), posZ(y), loaded(false), cachePacket(Network::OP_CHUNK_DATA), blockChangePacket(Network::OP_MULTI_BLOCK_CHANGE), flagSectionExists(0), flagSectionUseAdd(0), inCache(false), countChange(0)
{
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

void Chunk::ChangeBlock(int x, unsigned char y, int z, int blockID, int blockData)
{
    inCache = false;
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
void Chunk::ChangeData(int x, unsigned char y, int z, int blockData)
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

void Chunk::ResetBlockChangePacket()
{
    blockChangePacket.Reset();
    blockChangePacket << posX << posZ << (short)0 << (int)0;
    countChange = 0;
}

} /* namespace Network */
