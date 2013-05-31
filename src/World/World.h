#ifndef WORLD_H_
#define WORLD_H_

#include <unordered_map>
#include <string>
#include <set>

#include "Chunk.h"
#include "Entity/EntityConstants.h"
#include "RegionManager.h"
#include "Entity/Position.h"
#include "Util/AABB.h"
#include "Util/BufferedRewindableQueue.h"
#include "World/WorldConstants.h"

#include "Block/Scripts/Basics/BlockRedstoneTorchBurnoutMgr.h"

#define CHUNK_KEY(X,Z) ((((long)X << 32) & 0xffffffff00000000)| ((long)Z & 0x00000000ffffffff))

namespace Network
{
class NetworkPacket;
}
namespace nbt
{
class NbtBuffer;
class TagCompound;
class NbtFile;
}
namespace World
{

class Chunk;
class Position;
class Entity;
class EntityPlayer;
class VirtualChunk;
class VirtualSmallChunk;

class World
{
    friend Chunk;
public:
    World(const std::string& worldName);
    virtual ~World();

    void UpdateTick();

    void AddEntity(Entity* entity);
    void AddPlayer(EntityPlayer* entity);

    void RemoveEntity(Entity* entity);
    void RemovePlayer(EntityPlayer* entity);

    /*Space partitioning utils*/
    inline Chunk* GetChunk(int x, int z);
    inline Chunk* GetChunkIfLoaded(int x, int z) const;

    inline VirtualChunk* GetVirtualChunk(int x, int z);
    inline VirtualChunk* GetVirtualChunkIfLoaded(int x, int z) const;

    inline VirtualSmallChunk* GetVirtualSmallChunk(int x, int z);
    inline VirtualSmallChunk* GetVirtualSmallChunkIfLoaded(int x, int z) const;

    void RequestChunk(EntityPlayer* player, int x, int z);
    void Unload();
    int getViewDistance();
    void SetTime(long time);

    /*Blocks utils*/
    inline i_block GetBlockId(int x, i_height y, int z);
    inline i_data GetBlockData(int x, i_height y, int z);

    inline bool IsFullBlock(int x, i_height y, int z);

    inline s_block_data GetBlockIdAndData(int x, i_height y, int z) const;

    void ChangeDataNoEvent(int x, i_height y, int z, i_data blockData);
    void ChangeDataNotify(int x, i_height y, int z, i_data blockData);
    void ChangeBlockNoEvent(int x, i_height y, int z, i_block blockId, i_data blockData);
    void ChangeBlock(int x, i_height y, int z, i_block blockId, i_data blockData, bool playSound = true);
    void RemoveBlock(int x, i_height y, int z);
    void MarkBlockForUpdate(int x, i_height y, int z, i_block blockId, unsigned int waitTick = 1);


    /*Network/Notification to player functions*/
    void SendPacketToPlayerInWorld(const Network::NetworkPacket& packet) const;

    void PlaySound(double x, double y, double z, const std::wstring& soundName, float volume, char modifier, unsigned char distanceChunk);
    void PlaySoundOrParticleEffect(double x, i_height y, double z, int effectId, int data, bool disableRelativeVolume, unsigned char distanceChunk);
    void PlayBlockAction(int x, short y, int z, char type, char modifier, i_block blockId, char distanceChunk);
    void UpdateGameState(char reason, char gameMode);

    /*Item util*/
    void DropItemstackWithRandomDirection(double x, double y, double z, Inventory::ItemStack* itemstack);

    /*Entity collision/find functions*/
    void GetBlockBoundingBoxInRange1(int x, int y, int z, std::vector<Util::AABB>& bbList);
    void GetBlockBoundingBoxInRange(int x, int y, int z, int range, int rangeHeight, std::vector<Util::AABB>& bbList);
    void GetBlockBoundingBoxInAABB(const Util::AABB& box, std::vector<Util::AABB>& bbList) const;

    void GetEntitiesBoundingBoxInAABB(const std::set<eEntityType> &type, int ignoreEntityId, const Util::AABB& boundingBox, std::vector<std::pair<int, Util::AABB>>& bbList);
    void GetEntitiesBoundingBoxInAABB(int ignoreEntityId, const Util::AABB& boundingBox, std::vector<std::pair<int, Util::AABB>>& bbList);
    void GetEntitiesBoundingBoxInAABBByEntityType(eEntityType type, int ignoreEntityId, const Util::AABB& boundingBox, std::vector<std::pair<int, Util::AABB>>& bbList);
    void GetEntitiesBoundingBoxInAABBByEntityFlag(int entityTypeFlag, int ignoreEntityId, const Util::AABB& boundingBox, std::vector<std::pair<int, Util::AABB>>& bbList);

    void GetEntitiesInRangeByEntityType(eEntityType type, int ignoreEntityId, const Position& center, int range, std::vector<Entity*>& outEntityList);
    void GetEntitiesInAABB(int ignoreEntityId, const Util::AABB& box, std::vector<Entity*>& outEntityList);

    /*Entity managment*/
    void MarkEntityAsDead(int entityId);

    Entity* GetEntityById(int target);


    /*Light management*/
    i_lightopacity GetBlockLightOpacity(int x, i_height y, int z);
    i_lightvalue GetRealLightValueAt(int x, i_height y, int z);

    void MarkForNetworkUpdateTileEntity(int x, i_height y, int z);
    Block::TileEntity* GetTileEntity(int x, i_height y, int z);

    void NotifyNeighborsForBlockChange(int x, i_height y, int z, i_block blockId);
    void NotifyNeighborBlockChange(int x, i_height y, int z, i_block neighborBlockId);

    void NotifyTileEntityStateChange(int x, i_height y, int z, int action);

    bool IsNormalCube(int x, i_height y, int z);

    i_powerlevel getBlockPower(int x, i_height y, int z, int direction);
    i_powerlevel computePowerLevelFromAroundBlock(int x, i_height y, int z);
    bool isBlockIndirectlyProvidingPowerTo(int x, i_height y, int z, int direction);
    i_powerlevel getIndirectPowerLevel(int x, i_height y, int z, int direction);
    bool isBlockIndirectlyGettingPowered(int x, i_height y, int z);
    i_powerlevel getMaxPowerFromBlockArround(int x, i_height y, int z);

    Scripting::BlockRedstoneTorchBurnoutMgr* GetRedstoneTorchBurnoutMgr() const;
    const Position& GetSpawnPosition() const;
    Position GetValidSpawnPosition();
    nbt::NbtFile* LoadNbtDatasForPlayer(const std::string& playerName);

private:
    void UpdateTime();
    VirtualChunk* CreateVirtualChunk(int x, int z);
    VirtualSmallChunk* CreateVirtualSmallChunk(int x, int z);
    Chunk* LoadChunk(int x, int z);
    nbt::NbtBuffer* GetChunkNbtData(int x, int z);
    void MarkEntityForDelete(Entity* entity);
    void MarkPlayerForRemove(EntityPlayer* entity);

    bool isChunksExistInRange(int x, i_height y, int z, int range);
    bool isChunksExist(int xmin, i_height ymin, int zmin, int xmax, i_height ymax, int zmax);
    bool isChunkExist(int chunkX, int chunkZ);

    i_height getMinHeightAndHeightMapAt(int x, int z, i_height& heightMap);
    i_height getMinHeightMapAt(int x, int z);

    /*Light functions*/
    void updateAllLightTypes(int x, i_height y, int z);
    void updateLightByType(eLightType lightType, int x, i_height y, int z);
    void updateSkylightOnColumnt(int x, int z, i_height y1, i_height y2);
    i_lightvalue computeBlockLightValueUsingNeighbors(eLightType lightType, int x, i_height y, int z);
    i_lightvalue getLightValueAt(eLightType lightType, int x, i_height y, int z);
    void setLightValueAt(eLightType lightType, int x, i_height y, int z, i_lightvalue value);
    bool isBlockDirectlyLightedFromSky(int x, i_height y, int z);
    i_lightvalue recursiveGetRealLightValueAt(int x, i_height y, int z, bool firstCall);

    /**
     * Load level.dat
     */
    void load();

    /**
     * Load spawn data from nbt in level.dat
     * @param tagData data to load
     */
    void loadSpawn(nbt::TagCompound* tagData);

    /**
     * Load time and weather data from nbt in level.dat
     * Time of day...
     * Raining state/Rain time
     * Thunfer state/time
     * @param tagData data to load
     */
    void loadTimeAndWeather(nbt::TagCompound* tagData);

    /**
     * Load gamemode and some data from world nbt data in level.dat
     * Seed
     * Hardcore mode
     * GameType
     * @param tagData data to load
     */
    void loadGameMode(nbt::TagCompound* tagData);
private:
    /// Name of the world, it name also directory where files are store
    std::string worldName;
    /// Manager for regions files
    RegionManager regionManager;

    /// Map that store Chunk, use macro CHUNK_KEY(x, z) to get a chunk
    std::unordered_map<long, Chunk*> chunkMap;
    /// Map that store VirtualChunk, use macro CHUNK_KEY(x, z) to get a one
    std::unordered_map<long, VirtualChunk*> virtualChunkMap;
    /// Map that store VirtualSmallChunk, use macro CHUNK_KEY(x, z) to get a one
    std::unordered_map<long, VirtualSmallChunk*> virtualSmallChunkMap;


    /// Link EntityId and Entity
    std::map<int, Entity*> entityById;
    /// List of player in world
    std::set<EntityPlayer*> playerList;

    /// Mark dead entityId, that will be remove at the end of tick
    std::set<int> deadEntity;

    /// Entity which will be delete next tick
    std::vector<Entity*> entityToDelete;
    /// Player that will be remove at end of tick
    std::set<EntityPlayer*> playerToRemove;

    /// True if world is performing UpateTick()
    bool updateInProgress;

    /**
     * Distance of the player view in chunk, if 10, he will see 10 chunk in front of him/back etc... so 441 chunk
     * (10 + 1 + 10) * (10 + 1 + 10) = 441 chunk loaded around a player
     * (9 + 1 + 9) * (9 + 1 + 9) = 361 chunk loaded around a player
     * (8 + 1 + 8) * (8 + 1 + 8) = 289 chunk loaded around a player
     */
    int viewDistance;

    /// Id of next entity
    int currentEntityId;

    /*
     * Light utilities
     */
    struct LightUpdateData
    {
        int x:6;
        int y:6;
        int z:6;
        unsigned l:4;
    };
    Util::BufferedRewindableQueue<struct LightUpdateData, 32768> updateLightQueue;
    i_lightvalue sunReduceValue;

    /// Redstone manager for burnout
    Scripting::BlockRedstoneTorchBurnoutMgr* redstoneTorchBurnoutMgr;

    // TODO: Export it in an other class ?
    Position spawnPosition;

    long ageOfWorld;
    long currentTime;

    int rainTime;
    bool raining;
    int thunderTime;
    bool thundering;

    bool hardcore;
    long seed;
    int gameType;
};

} /* namespace World */

#include "World.hxx"

#endif /* WORLD_H_ */
