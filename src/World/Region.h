#ifndef REGION_H_
#define REGION_H_

#include <fstream>
#include <vector>

#include "Util/types.h"

namespace NBT
{
class TagCompound;
}
namespace World
{

#define REGION_BLOCK_SIZE 4096
#define REGION_CHUNK_WIDTH 32
#define REGION_CHUNK_HEIGHT 32
#define REGION_CHUNK_COUNT REGION_CHUNK_WIDTH * REGION_CHUNK_HEIGHT
#define REGION_HEADER_SIZE size_t(REGION_CHUNK_COUNT * 2 * sizeof(int))

class Region
{
public:
    Region(const std::string& worldPath, int x, int z);
    virtual ~Region();

    NBT::TagCompound* GetNbtChunkData(i_small_coord chunkX, i_small_coord chunkZ);
    void SaveNbtChunkData(i_small_coord chunkX, i_small_coord chunkZ, NBT::TagCompound* nbtChunkData);
private:
    union offset
    {
        int value;
        struct __attribute__((__packed__)) {
            unsigned int size : 8;
            unsigned int offsetBlock : 24;
        } data;
    };
private:
    int regionX;
    int regionZ;
    offset locationsTable[REGION_CHUNK_HEIGHT][REGION_CHUNK_WIDTH];
    int lastAccessTimeTable[REGION_CHUNK_HEIGHT][REGION_CHUNK_WIDTH];
    std::vector<bool> freeBlock;
    std::fstream file;
    bool opened;
};

} /* namespace World */
#endif /* REGION_H_ */
