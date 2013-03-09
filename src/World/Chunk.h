#ifndef CHUNK_H_
#define CHUNK_H_

#include <map>
#include <set>
#include <vector>
#include <iostream>

#define CHUNK_SURFACE 256
#define CHUNK_DATA_COUNT 16
#define CHUNK_BLOCK_COUNT (16 * 16 * 16)
#define CHUNK_BLOCK_NIBBLE_SIZE CHUNK_BLOCK_COUNT / 2

#include "Util/types.h"
#include "Network/NetworkPacket.h"

namespace Block
{
class TileEntity;
}
namespace World
{

class EntityPlayer;
/*
 * This class store chunk data, block, metadata, light, tileentities etc...
 * A chunk is a block of 16 x 16 x 256, divide in 16 block of 16 x 16 x 16
 * All coordinate must be 0 <= [xz] <= 15
 */
class Chunk
{
public:
    Chunk(int x, int y);
    virtual ~Chunk();

    // Get block id or data at coordninate
    inline i_block getBlockAt(i_small_coord x, i_height y, i_small_coord z);
    inline i_data getDataAt(i_small_coord x, i_height y, i_small_coord z);

    // Write in chunk data without notification for client or other block
    inline void SetBlockAt(i_small_coord x, i_height y, i_small_coord z, i_block blockID);
    inline void SetDataAt(i_small_coord x, i_height y, i_small_coord z, i_data newData);

    // Load chunk from file
    void Load();
    // Unload the chunk, save and free data
    void Unload();

    // Add player to chunk, to be notified of each change that produce in this
    void AddPlayer(EntityPlayer* player);
    // Remove player from chunk, player disconect or move to far
    void RemovePlayer(EntityPlayer* player);

    // Called each tick to update block
    void UpdateTick();

    // Generate or get cached packet witch containing chunk datas
    const Network::NetworkPacket& GetPacket();

    // Modifiy a block or data only, and mark them to be notified to client
    void ChangeBlock(i_small_coord x, i_height y, i_small_coord z, i_block blockID, i_data blockData);
    void ChangeData(i_small_coord x, i_height y, i_small_coord z, i_data blockData);

    // Send all change done with ChangeBlock or ChangeData to client
    void SendUpdate();

    // Add new tile entity
    void SetTileEntity(Block::TileEntity* tileEntity, i_small_coord x, i_height y, i_small_coord z);
    // Delete a tileentity
    void RemoveTileEntity(i_small_coord x, i_height y, i_small_coord z);
    // Retrive tile entity in block
    Block::TileEntity* GetTileEntity(i_small_coord x, i_height y, i_small_coord z);
private:
    void GeneratePacket();
    void ResetBlockChangePacket();
private:
    // 16 x 16 x 16
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
    std::map<unsigned short, Block::TileEntity*> tileEntities;
};


} /* namespace World */

#include "Chunk.hxx"
#endif /* CHUNK_H_ */
