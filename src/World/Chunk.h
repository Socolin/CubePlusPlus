#ifndef CHUNK_H_
#define CHUNK_H_

namespace World
{
#define CHUNK_DATA_COUNT 8
#define CHUNK_BLOCK_COUNT 16 * 16 * 16
#define CHUNK_BLOCK_NIBBLE_SIZE CHUNK_BLOCK_COUNT / 2

class Chunk
{
public:
	Chunk();
	virtual ~Chunk();
	int getBlockAt(int x, int y, int z);
private:
	typedef struct
	{
		unsigned char blocks[CHUNK_BLOCK_COUNT];
		unsigned char metadata[CHUNK_BLOCK_NIBBLE_SIZE];
		unsigned char* addData;
	} ChunkData;

	ChunkData* datas[CHUNK_DATA_COUNT];
};

} /* namespace Network */
#endif /* CHUNK_H_ */
