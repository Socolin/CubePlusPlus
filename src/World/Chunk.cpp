/*
 * Chunk.cpp
 *
 *  Created on: 5 déc. 2012
 *      Author: bertrand
 */

#include "Chunk.h"

#include <stdlib.h>

namespace World
{

Chunk::Chunk(int x, int y) :
        posX(x), posY(y), refCount(0), loaded(false)
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
    for (int i = 0; i < CHUNK_DATA_COUNT; i++)
    {
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
        chunkData->addData = NULL;
        datas[i] = chunkData;
    }
    {
        ChunkData* chunkData =  datas[0];
        unsigned char* data = chunkData->blocks;
        for (int i = 0; i < CHUNK_BLOCK_COUNT; i++)
        {
            *data = 1;
            data++;
        }
        data = chunkData->metadata;
        for (int i = 0; i < CHUNK_BLOCK_NIBBLE_SIZE; i++)
        {
            *data = 0;
            data++;
        }
        chunkData->addData = NULL;
        datas[0] = chunkData;
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

} /* namespace Network */
