#ifndef CHUNK_H_
#define CHUNK_H_

#include <set>
#include <iostream>

#define CHUNK_SURFACE 256
#define CHUNK_DATA_COUNT 16
#define CHUNK_BLOCK_COUNT (16 * 16 * 16)
#define CHUNK_BLOCK_NIBBLE_SIZE CHUNK_BLOCK_COUNT / 2

#include "Network/NetworkPacket.h"

namespace World
{

class Chunk
{
public:
    Chunk(int x, int y);
    virtual ~Chunk();

    inline int getBlockAt(int x, int y, int z)
    {
        ChunkData* data = datas[y << 4];
        if (data != NULL)
        {
            if (data->addData != NULL)
            {
                int cellId = (y << 8 | z << 4 | x);
                return data->blocks[cellId] | (data->addData[cellId << 2] & (0xf << ((x & 0x1) << 2)));
            }
            else
            {
                return data->blocks[y << 8 | z << 4 | x];
            }
        }
        return 0;
    }
    inline int getDataAt(int x, int y, int z)
    {
        ChunkData* data = datas[y << 4];
        if (data != NULL)
        {
            return data->metadata[(y << 8 | z << 4 | x) << 2] & (0xf << ((x & 0x1) << 2));
        }
        return 0;
    }

    void Load();
    inline void AddRefCount()
    {
        refCount++;
    }
    inline void RemoveRefCount()
    {
        refCount--;
    }
    void UpdateTick();
    void Unload();

    const Network::NetworkPacket& GetPacket() const;

private:
    typedef struct
    {
        unsigned char blocks[CHUNK_BLOCK_COUNT];
        unsigned char metadata[CHUNK_BLOCK_NIBBLE_SIZE];
        unsigned char blocklight[CHUNK_BLOCK_NIBBLE_SIZE];
        unsigned char skyLight[CHUNK_BLOCK_NIBBLE_SIZE];
        unsigned char* addData;
    } ChunkData;

    ChunkData* datas[CHUNK_DATA_COUNT];
    int posX;
    int posZ;
    int refCount;
    bool loaded;
    Network::NetworkPacket cachePacket;
    unsigned short flagSectionExists;
    unsigned short flagSectionUseAdd;
    unsigned char biomeData[CHUNK_SURFACE];
};

} /* namespace Network */
#endif /* CHUNK_H_ */
