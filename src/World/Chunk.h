#ifndef CHUNK_H_
#define CHUNK_H_

#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <boost/heap/binomial_heap.hpp>

#define CHUNK_SURFACE 256
#define CHUNK_DATA_COUNT 16
#define CHUNK_BLOCK_COUNT (16 * 16 * 16)
#define CHUNK_BLOCK_NIBBLE_SIZE CHUNK_BLOCK_COUNT / 2

#include "Util/types.h"
#include "Network/NetworkPacket.h"
#include "WorldConstants.h"

namespace nbt
{
class NbtBuffer;
}
namespace Block
{
class TileEntity;
}
namespace World
{

class EntityPlayer;
class World;
/**
 * This class store chunk data, block, metadata, light, tileentities etc...
 * A chunk is a block of 16 x 16 x 256, divide in 16 block of 16 x 16 x 16
 * All coordinate must be 0 <= [xz] <= 15
 */
class Chunk
{
    friend World;
public:
    Chunk(int x, int y, World* world);
    virtual ~Chunk();

    // Get block id or data at coordninate
    inline i_block getBlockAt(i_small_coord x, i_height y, i_small_coord z);
    inline i_data getDataAt(i_small_coord x, i_height y, i_small_coord z);

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

    // Get data from tile entities
    void GetTileEntityPacket(Network::NetworkPacket& packet);

    // Send all change done with ChangeBlock or ChangeData to client
    void SendUpdate();

    // Add new tile entity
    void SetTileEntity(Block::TileEntity* tileEntity, i_small_coord x, i_height y, i_small_coord z);
    // Mark for update tile entity
    void MarkForNetworkUpdateTileEntity(i_small_coord x, i_height y, i_small_coord z);
    // Delete a tileentity
    void RemoveTileEntity(i_small_coord x, i_height y, i_small_coord z);
    // Retrive tile entity in block
    Block::TileEntity* GetTileEntity(i_small_coord x, i_height y, i_small_coord z);
    i_height getMinHeight() const;

    void ChangeBlockNoEventNoTileEntityChange_DoNotUseExceptIfYouKnowWhatYouDo(i_small_coord x, i_height y, i_small_coord z, i_block blockID, i_data blockData);
private:

    inline i_lightvalue getBlockLightAt(i_small_coord x, i_height y, i_small_coord z);
    inline i_lightvalue getSkyLightAt(i_small_coord x, i_height y, i_small_coord z);
    i_lightvalue getRealBlockLightValue(i_small_coord x, i_height y, i_small_coord z, i_lightvalue sunReduceValue);
    inline void setBlockLightAt(i_small_coord x, i_height y, i_small_coord z, i_lightvalue value);
    inline void setSkyLightAt(i_small_coord x, i_height y, i_small_coord z, i_lightvalue value);

    inline int getHeightMapAt(i_small_coord x, i_small_coord z);
    i_height getFirstAvaibleChunkDataPositionFromTop();

    // Write in chunk data without notification for client or other block
    inline void SetBlockAt(i_small_coord x, i_height y, i_small_coord z, i_block blockID);
    inline void SetDataAt(i_small_coord x, i_height y, i_small_coord z, i_data newData);

    void updateHeightMapAndSkyLight(i_small_coord x, i_height y, i_small_coord z);

    // Modifiy a block or data only, and mark them to be notified to client
    void ChangeBlock(i_small_coord x, i_height y, i_small_coord z, i_block blockID, i_data blockData);
    void ChangeData(i_small_coord x, i_height y, i_small_coord z, i_data blockData);

    void GeneratePacket();
    void ResetBlockChangePacket();

    void MarkForUpdate(i_small_coord x, i_height y, i_small_coord z, i_block blockId, unsigned int tickWait = 1);

    void UpdateSkyLightIFN();
    void propagateSkylightOcclusion(i_small_coord x, i_small_coord z);
    void GenerateSkyLight();

    i_lightopacity getBlockLightOpacity(i_small_coord x, i_height y, i_small_coord z);

    bool loadFromFile(nbt::NbtBuffer* nbtData);
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
            addData = nullptr;
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

    inline ChunkData* getOrCreateData(i_small_coord y)
    {
        ChunkData* data = datas[y];
        if (data == NULL)
        {
            data = new ChunkData();
            data->clear();
            flagSectionExists |= (1 << y);
            datas[y] = data;
        }
        return data;
    }

    inline ChunkData* getOrCreateDataForLight(i_small_coord y)
    {
        ChunkData* data = datas[y];
        if (data == NULL)
        {
            data = new ChunkData();
            data->clear();
            flagSectionExists |= (1 << y);
            datas[y] = data;
            GenerateSkyLight();
        }
        return data;
    }
    struct UpdateBlockCoordStruct
    {
        i_small_coord x: 4;
        i_small_coord z: 4;
        i_small_coord y: 4;
        unsigned char chunkDataY: 4;
    } __attribute__((packed));
    union UpdateBlockCoord
    {
        struct UpdateBlockCoordStruct coord;
        unsigned short cellId;
    };
    typedef struct __attribute__((packed))
    {
        unsigned int updateTick;
        union UpdateBlockCoord coord;
        unsigned short blockId;
    } UpdateBlockData;
    struct CompateUpdateBlockData
    {
       bool operator() (const UpdateBlockData &a,const UpdateBlockData &b) const
       {
           return (a.updateTick > b.updateTick);
       }
     };

    ChunkData* datas[CHUNK_DATA_COUNT];
    int posX;
    int posZ;
    int posXx16;
    int posZx16;
    bool loaded;
    Network::NetworkPacket cachePacket;
    Network::NetworkPacket blockChangePacket;
    unsigned short flagSectionExists;
    unsigned short flagSectionUseAdd;
    unsigned char biomeData[CHUNK_SURFACE];
    unsigned char heightMap[CHUNK_SURFACE];
    bool inCache;
    std::vector<unsigned int> changedBlock;
    short countChange;
    std::set<EntityPlayer*> playerList;
    std::map<unsigned short, Block::TileEntity*> tileEntities;
    std::set<Block::TileEntity*> activeTileEntities;
    std::set<Block::TileEntity*> updatedTileEntities;
    Network::NetworkPacket tileEntityUpdatePacket;
    World* world;
    unsigned int selfTickCounter;
    boost::heap::binomial_heap<UpdateBlockData,boost::heap::compare<CompateUpdateBlockData>> toUpdateBlockList;
    i_height minHeight;
    bool skylightNeedUpdate;
    bool skylightColumnsToUpdate[CHUNK_SURFACE];
};


} /* namespace World */

#include "Chunk.hxx"
#endif /* CHUNK_H_ */
