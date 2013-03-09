#ifndef CHUNK_HXX_
#define CHUNK_HXX_

namespace World
{
i_block Chunk::getBlockAt(i_small_coord x, i_height y, i_small_coord z)
{
	ChunkData* data = datas[y >> 4];
	if (data != NULL)
	{
		if (data->addData != NULL)
		{
			int cellId = ((y & 0xf) << 8 | z << 4 | x);
			return data->blocks[cellId] | (data->addData[cellId << 1] & (0xf << ((x & 0x1) << 2)));
		}
		else
		{
			return data->blocks[(y & 0xf) << 8 | z << 4 | x];
		}
	}
	return 0;
}
i_data Chunk::getDataAt(i_small_coord x, i_height y, i_small_coord z)
{
	ChunkData* data = datas[y >> 4];
	if (data != NULL)
	{
		int cellId = (y & 0xf) << 8 | z << 4 | x;
		if ((x & 0x1) == 0)
		{
			return data->metadata[cellId >> 1] & 0xf;
		}
		else
		{
			return (data->metadata[cellId >> 1] & 0xf0) >> 4;
		}
	}
	return 0;
}

void Chunk::SetBlockAt(i_small_coord x, i_height y, i_small_coord z, i_block blockID)
{
	ChunkData* data = datas[y >> 4];
	if (data == NULL)
	{
		data = new ChunkData();
		data->clear();
		flagSectionExists |= (1 << (y >> 4));
		datas[y >> 4] = data;
	}
	if (blockID > 255)
	{
		if (data->addData == NULL)
		{
			data->addData = new unsigned char[CHUNK_BLOCK_NIBBLE_SIZE];
			data->clearAddData();
			flagSectionUseAdd |= (1 << (y >> 4));
		}
		int cellId = ((y & 0xf) << 8 | z << 4 | x);
		data->blocks[cellId] = blockID & 0xff;

		i_data currentBlock = data->addData[cellId >> 1];
		if ((x & 0x1) == 0)
		{
			data->addData[cellId >> 1] = (currentBlock & 0xf0) | ((blockID >> 8) & 0xf);
		}
		else
		{
			data->addData[cellId >> 1] = (currentBlock & 0xf) | (((blockID >> 8) & 0xf) << 4);// TODO: ((blockID >> 4) & 0xf0)
		}
	}
	else
	{
		data->blocks[(y & 0xf) << 8 | z << 4 | x] = blockID & 0xff;
	}
}
void Chunk::SetDataAt(i_small_coord x, i_height y, i_small_coord z, i_data newData)
{
	ChunkData* data = datas[y >> 4];
	if (data != NULL)
	{
		int cellId = (y & 0xf) << 8 | z << 4 | x;

		i_data currentData = data->metadata[cellId >> 1];
		if ((x & 0x1) == 0)
		{
			data->metadata[cellId >> 1] = (currentData & 0xf0) | (newData & 0xf);
		}
		else
		{
			data->metadata[cellId >> 1] = (currentData & 0xf) | ((newData & 0xf) << 4);
		}
	}
}
}

#endif
