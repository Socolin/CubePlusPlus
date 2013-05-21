#ifndef REGION_H_
#define REGION_H_

#include <fstream>
#include <vector>

#include "Util/types.h"

namespace nbt
{
class NbtBuffer;
}
namespace World
{

#define REGION_BLOCK_SIZE 4096
#define REGION_CHUNK_WIDTH 32
#define REGION_CHUNK_HEIGHT 32
#define REGION_CHUNK_COUNT REGION_CHUNK_WIDTH * REGION_CHUNK_HEIGHT
#define REGION_HEADER_SIZE REGION_CHUNK_COUNT * 2 * sizeof(int)

class Region
{
public:
    Region(const std::string& worldPath, int x, int z);
    virtual ~Region();

    nbt::NbtBuffer* GetNbtChunkData(size_t& size, i_small_coord chunkX, i_small_coord chunkZ);
private:
    union offset
    {
        int value;
        struct __attribute__((__packed__)) {
            int size : 8;
            int offsetBlock : 24;
        } data;
    };
private:
    int regionX;
    int regionZ;
    offset locationsTable[REGION_CHUNK_HEIGHT][REGION_CHUNK_WIDTH];
    int lastAccessTimeTable[REGION_CHUNK_HEIGHT][REGION_CHUNK_WIDTH];
    std::vector<bool> freeBlock;
    std::fstream file;
};

} /* namespace World */
#endif /* REGION_H_ */
