/*
 * Chunk.cpp
 *
 *  Created on: 5 déc. 2012
 *      Author: bertrand
 */

#include "Chunk.h"

#include <iostream>

#include "Network/OpcodeList.h"

namespace World
{

Chunk::Chunk(int x, int y) :
        posX(x), posZ(y), refCount(0), loaded(false), cachePacket(Network::OP_CHUNK_DATA), flagSectionExists(0), flagSectionUseAdd(0)
{
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
        datas[i] = NULL;
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
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
    {
        flagSectionExists |= (1 << i);
        ChunkData* chunkData = new ChunkData();
        unsigned char* data = chunkData->blocks;
        for (int i = 0; i < CHUNK_BLOCK_COUNT; i++)
        {
            *data = 0;
            data++;
        }
        data = chunkData->metadata;
        for (int i = 0; i < CHUNK_BLOCK_NIBBLE_SIZE; i++)
        {
            *data = 0;
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
        datas[i] = chunkData;
    }

    {
        unsigned char* data = biomeData;
        for (int i = 0; i < CHUNK_SURFACE; i++)
        {
            *data = 0;
            data++;
        }
    }
    {
        ChunkData* chunkData =  datas[0];
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
        chunkData->addData = NULL;
    }

    {
        ChunkData* chunkData =  datas[1];
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
        chunkData->addData = NULL;
    }
    loaded = true;
    cachePacket << posX << posZ << true << flagSectionExists << flagSectionUseAdd;
    cachePacket.startWriteCompressedData();
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
    {
        ChunkData* chunkData =  datas[i];
        cachePacket.appendCompress((char*)chunkData->blocks,CHUNK_BLOCK_COUNT);
    }
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
    {
        ChunkData* chunkData =  datas[i];
        cachePacket.appendCompress((char*)chunkData->metadata,CHUNK_BLOCK_NIBBLE_SIZE);
    }
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
    {
        ChunkData* chunkData =  datas[i];
        cachePacket.appendCompress((char*)chunkData->blocklight,CHUNK_BLOCK_NIBBLE_SIZE);
    }
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
    {
        ChunkData* chunkData =  datas[i];
        cachePacket.appendCompress((char*)chunkData->skyLight,CHUNK_BLOCK_NIBBLE_SIZE);
    }

    cachePacket.appendCompress((char*)biomeData,CHUNK_SURFACE);

    //cachePacket.dump();
    cachePacket.endWriteCompressedData();
}

void Chunk::UpdateTick()
{
    if (!loaded)
        return;
}

void Chunk::Unload()
{
}

const Network::NetworkPacket& Chunk::GetPacket() const
{
    return cachePacket;
}

} /* namespace Network */
