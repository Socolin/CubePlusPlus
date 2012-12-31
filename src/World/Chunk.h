#ifndef CHUNK_H_
#define CHUNK_H_

#include <set>
#include <vector>
#include <iostream>

#define CHUNK_SURFACE 256
#define CHUNK_DATA_COUNT 16
#define CHUNK_BLOCK_COUNT (16 * 16 * 16)
#define CHUNK_BLOCK_NIBBLE_SIZE CHUNK_BLOCK_COUNT / 2

#include "Network/NetworkPacket.h"

namespace World
{

class EntityPlayer;
class Chunk
{
public:
    Chunk(int x, int y);
    virtual ~Chunk();

    inline int getBlockAt(int x, int y, int z)
    {
        ChunkData* data = datas[y >> 4];
        if (data != NULL)
        {
            if (data->addData != NULL)
            {
                int cellId = (y << 8 | z << 4 | x);
                return data->blocks[cellId] | (data->addData[cellId << 1] & (0xf << ((x & 0x1) << 2)));
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
        ChunkData* data = datas[y >> 4];
        if (data != NULL)
        {
            return data->metadata[(y << 8 | z << 4 | x) << 1] & (0xf << ((x & 0x1) << 2));
        }
        return 0;
    }

    inline void SetBlockAt(int x, int y, int z, int blockID)
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
            }
            int cellId = ((y & 0xf) << 8 | z << 4 | x);
            data->blocks[cellId] = blockID & 0xff;

            unsigned char currentData = data->addData[cellId >> 1];
            if ((x & 0x1) == 0)
            {
                data->addData[cellId >> 1] = (currentData & 0xf0) | ((blockID >> 8) & 0xf);
            }
            else
            {
                data->addData[cellId >> 1] = (currentData & 0xf) | (((blockID >> 8) & 0xf) << 4);
            }
            flagSectionUseAdd |= (1 << (y >> 4));
        }
        else
        {
            data->blocks[(y & 0xf) << 8 | z << 4 | x] = blockID & 0xff;
        }
    }
    inline void SetDataAt(int x, int y, int z, unsigned char newData)
    {
        ChunkData* data = datas[y >> 4];
        if (data != NULL)
        {
            int cellId = (y & 0xf) << 8 | z << 4 | x;

            unsigned char currentData = data->metadata[cellId >> 1];
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

    void Load();
    void AddPlayer(EntityPlayer* player);
    void RemovePlayer(EntityPlayer* player);
    void UpdateTick();
    void Unload();

    const Network::NetworkPacket& GetPacket();

    void ChangeBlock(int x, unsigned char y, int z, int blockID, int blockData);
    void SendUpdate();
private:
    void GeneratePacket();
    void ResetBlockChangePacket();
private:
    typedef struct
    {
        unsigned char blocks[CHUNK_BLOCK_COUNT];
        unsigned char metadata[CHUNK_BLOCK_NIBBLE_SIZE];
        unsigned char blocklight[CHUNK_BLOCK_NIBBLE_SIZE];
        unsigned char skyLight[CHUNK_BLOCK_NIBBLE_SIZE];
        unsigned char* addData;

        void clear()
        {
            unsigned char* data = blocks;
            for (int i = 0; i < CHUNK_BLOCK_COUNT; i++)
            {
                *data = 0;
                data++;
            }
            data = metadata;
            for (int i = 0; i < CHUNK_BLOCK_NIBBLE_SIZE; i++)
            {
                *data = 0;
                data++;
            }
            data = blocklight;
            for (int i = 0; i < CHUNK_BLOCK_NIBBLE_SIZE; i++)
            {
                *data = 0;
                data++;
            }
            data = skyLight;
            for (int i = 0; i < CHUNK_BLOCK_NIBBLE_SIZE; i++)
            {
                *data = 0xff;
                data++;
            }
            addData = NULL;
        }
    } ChunkData;

    ChunkData* datas[CHUNK_DATA_COUNT];
    int posX;
    int posZ;
    bool loaded;
    Network::NetworkPacket cachePacket;
    Network::NetworkPacket blockChangePacket;
    unsigned short flagSectionExists;
    unsigned short flagSectionUseAdd;
    unsigned char biomeData[CHUNK_SURFACE];
    bool inCache = false;
    std::vector<unsigned int> changedBlock;
    short countChange;
    std::set<EntityPlayer*> playerList;
};

} /* namespace Network */
#endif /* CHUNK_H_ */
