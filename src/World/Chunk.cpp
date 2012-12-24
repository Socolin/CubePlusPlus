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

Chunk::Chunk()
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

int Chunk::getBlockAt(int x, int y, int z)
{
	int chunkDataId = (y & 0xff) << 4;
	ChunkData* data = datas[chunkDataId];
	if (data != NULL)
	{
		int subChunkCoordX = x & 0xf;
		int subChunkCoordY = y & 0xf;
		int subChunkCoordZ = z & 0xf;
		if (data->addData != NULL)
		{
			int cellId = ((subChunkCoordY) << 8 | (subChunkCoordZ) << 4 | (subChunkCoordX));
			return data->blocks[cellId] | (data->addData[cellId << 2] & 0xf << ((subChunkCoordX & 0x1) << 2));
		}
		else
		{
			return data->blocks[(subChunkCoordY) << 8 | (subChunkCoordZ) << 4 | (subChunkCoordX)];
		}
	}
	return 0;
}

} /* namespace Network */
