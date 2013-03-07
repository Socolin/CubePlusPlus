#ifndef CHUNK_H_
#define CHUNK_H_

#include <set>
#include <vector>
#include <iostream>

#define CHUNK_SURFACE 256
#define CHUNK_DATA_COUNT 16
#define CHUNK_BLOCK_COUNT (16 * 16 * 16)
#define CHUNK_BLOCK_NIBBLE_SIZE CHUNK_BLOCK_COUNT / 2

#include "Util/types.h"
#include "Network/NetworkPacket.h"

namespace World
{

class EntityPlayer;
class Chunk
{
public:
    Chunk(int x, int y);
    virtual ~Chunk();

    inline i_block getBlockAt(int x, i_height y, int z)
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
    inline i_data getDataAt(int x, i_height y, int z)
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

    inline void SetBlockAt(int x, i_height y, int z, i_block blockID)
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
    inline void SetDataAt(int x, i_height y, int z, i_data newData)
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

    void Load();
    void AddPlayer(EntityPlayer* player);
    void RemovePlayer(EntityPlayer* player);
    void UpdateTick();
    void Unload();

    const Network::NetworkPacket& GetPacket();

    void ChangeBlock(int x, unsigned char y, int z, int blockID, int blockData);
    void ChangeData(int x, unsigned char y, int z, int blockData);
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
        void clearAddData()
        {
            unsigned char* data = addData;
            for (int i = 0; i < CHUNK_BLOCK_NIBBLE_SIZE; i++)
            {
                *data = 0xff;
                data++;
            }
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
