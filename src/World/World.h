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

#define CHUNK_KEY(X, Z) (((((long long)X) << 32) & 0xffffffff00000000)| (((long long)Z) & 0x00000000ffffffff))

namespace Network
{
class NetworkPacket;
}
namespace NBT
{
class Buffer;
class File;
class TagCompound;
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

    /*************************************************************************
     * General world managmenet
     *************************************************************************/

    /**
     * Called each tick to update all chunk, entities, and send update to all player.
     */
    void UpdateTick();

    /**
     * Save the world.
     * World data are save in <WorldPath>/level.dat
     * Players are save in <WorldPath>/players/
     * Chunk are save in <WorldPath>/regions/
     */
    void Save() const;




    /*************************************************************************
     * Entities managmenet
     *************************************************************************/

    /**
     * Add an entity to the world, and give an unique Id to here by calling Entity::SetWorld
     * Call Entity::OnJoinWorld when entity fully join the world.
     * @param entity
     */
    void AddEntity(Entity* entity);

    /**
     * Add a player to the world, and give an unique Id to here by calling Entity::SetWorld
     * Call EntityPlayer::OnJoinWorld when player fully join the world.
     * @param entity
     */
    void AddPlayer(EntityPlayer* entity);

    /**
     * Ask to remove player from world, it will be done at the end of the next tick.
     * @param player player to remove
     */
    void MarkPlayerForRemove(EntityPlayer* player);

    /**
     * Signal to world that the enity is now dead and must be remove from world
     * @param entityId id of entity to remove
     */
    void MarkEntityAsDead(int entityId);

    /**
     * Find an entity in world using its id
     * @param entityId
     * @return Pointer to entity or nullptr
     */
    Entity* GetEntityById(int entityId);

    /**
     * Get virtual chunk using virtual chunk coordinate, if it does not exist
     * then create then
     * @param x virtual chunk coordinate
     * @param z virtual chunk coordinate
     * @return a valid VirtualChunk pointer
     */
    inline VirtualChunk* GetVirtualChunk(int x, int z);

    /**
     * Get virtual chunk using virtual chunk coordinate, if it does not exist
     * then return nullptr
     * @param x virtual chunk coordinate
     * @param z virtual chunk coordinate
     * @return a valid VirtualChunk pointer or nullptr
     */
    inline VirtualChunk* GetVirtualChunkIfLoaded(int x, int z) const;

    /**
     * Get virtual chunk using small virtual chunk coordinate, if it does not exist
     * then create then
     * @param x virtual chunk coordinate
     * @param z virtual chunk coordinate
     * @return a valid VirtualSmallChunk pointer
     */
    inline VirtualSmallChunk* GetVirtualSmallChunk(int x, int z);

    /**
     * Get virtual chunk using small virtual chunk coordinate, if it does not exist
     * then return nullptr
     * @param x virtual chunk coordinate
     * @param z virtual chunk coordinate
     * @return a valid VirtualSmallChunk pointer or nullptr
     */
    inline VirtualSmallChunk* GetVirtualSmallChunkIfLoaded(int x, int z) const;

    /**
     * Load player data from file in <WorldPath>/players/<playerName>.dat
     * if data can't be load it return nullptr
     * @param playerName Case sensitive name player to load file
     * @return player data in nbt format, or nullptr
     */
    NBT::TagCompound* LoadNbtDatasForPlayer(const std::string& playerName);

    /**
     * Save player data to file file in <WorldPath>/players/<playerName>.dat
     * @param playerName name of player to save (case sensitive)
     * @param tagData nbt data of player see Entity::Save to fill it
     */
    void SaveNbtDatasForPlayer(const std::string& playerName, NBT::TagCompound* tagData) const;




    /*************************************************************************
     * Chunks managmenet
     *************************************************************************/

    /**
     * Get a pointer to a valid chunk, if chunk is not available yet, the it will
     * be load, or if load fail, it will be generate.
     * @param x x coordinate of chunk
     * @param z z coordinate of chunk
     * @return a valid Chunk pointer
     */
    inline Chunk* GetChunk(int x, int z);

    /**
     * Get a pointer to a valid chunk, if chunk is not available then return nullptr
     * @param x x coordinate of chunk
     * @param z z coordinate of chunk
     * @return a valid Chunk pointer or nullptr
     */
    inline Chunk* GetChunkIfLoaded(int x, int z) const;

    /**
     * Ask to send chunk data to a player, it contains blocks,light.. datas and
     * tileEntities data
     * @param player player who ask the chunk
     * @param x chunk coordinate
     * @param z chunk coordinate
     */
    void RequestChunk(EntityPlayer* player, int x, int z);




    /*************************************************************************
     * Weather and time managmenet
     *************************************************************************/

    /**
     * Change world current time (do not modify ageOfWorld)
     * @param time new current world time.
     */
    void SetTime(long long time);

    /**
     * Get the age of the world, age of the world is count in tick (20 tick/sec)
     * The value contain the total elapsed time of the world
     */
    long long GetAgeOfWorld() const;

    /**
     * Get the current time of the world, this define day or night
     */
    long long GetCurrentTime() const;




    /*************************************************************************
     * World parameters
     *************************************************************************/

    /**
     * Get number of chunk, that can see player.
     * Ex: If distance is 2, then player will receive all chunk around him
     * like in the draw below:
     *  ---------------------  ^
     *  |   |   |   |   |   |  | 2
     *  ---------------------  |
     *  |   |   |   |   |   |  V
     *  ---------------------
     *  |   |   | P |   |   |
     *  ---------------------
     *  |   |   |   |   |   |  ^
     *  ---------------------  |
     *  |   |   |   |   |   |  | 2
     *  ---------------------  V
     *  <------>     <------>
     *     2             2
     * @return distance in chunk, that can see player
     */
    int GetViewDistance() const;

    /**
     * Return the game mode loaded from level.dat.
     * @return world gamemode
     */
    int GetGameType() const;

    /**
     * Return if the world is in read only mode, or not.
     * In read only, world will not be save.
     * @return
     */
    bool IsReadOnly() const;

    /**
     * Get spawn position of world (loaded from level.dat)
     * @return Position of spawn
     */
    const Position& GetSpawnPosition() const;

    /**
     * Compute a valid spawn position using spawn position
     * @return valid spawn position
     */
    Position GetValidSpawnPosition();




    /*************************************************************************
     * Block management
     *************************************************************************/

    /**
     * Get id of block at given coordinate. If coordinates are in chunk not loaded
     * yet, then return 0, else it return the id of block
     * @param x coordinate of block
     * @param y coordinate of block 0 <= y < 256
     * @param z coordinate of block
     * @return id of blok (0 <= id < 4096)
     */
    inline i_block GetBlockId(int x, i_height y, int z);

    /**
     * Get metadata of block at given coordinate. If coordinates are in chunk not loaded
     * yet, then return 0, else it return the data of block
     * @param x coordinate of block
     * @param y coordinate of block 0 <= y < 256
     * @param z coordinate of block
     * @return the block data (0 <= data < 16)
     */
    inline i_data GetBlockData(int x, i_height y, int z);

    /**
     * Get the id and data of a block at given coordinate.  If coordinates are in chunk not loaded
     * yet, then return 0.
     * @param x coordinate of block
     * @param y coordinate of block 0 <= y < 256
     * @param z coordinate of block
     * @return the block id and data
     */
    inline s_block_data GetBlockIdAndData(int x, i_height y, int z) const;

    /**
     * Change block id and data at give coordinate
     * Notify all neighbor that the block change
     * Can play sound of new block when it is place
     * @param x coordinate of block
     * @param y coordinate of block 0 <= y < 256
     * @param z coordinate of block
     * @param blockId new block Id
     * @param blockData new block data
     * @param playSound play place sound of new block
     */
    void ChangeBlock(int x, i_height y, int z, i_block blockId, i_data blockData, bool playSound = true);

    /**
     * Change id and data at given coordinate, but do not notify it to
     * neighbors blocks
     * @param x coordinate of block
     * @param y coordinate of block 0 <= y < 256
     * @param z coordinate of block
     * @param blockId new block Id
     * @param blockData new block data
     */
    void ChangeBlockNoEvent(int x, i_height y, int z, i_block blockId, i_data blockData);

    /**
     * Change data of block, and notify all neighbor that the block change
     * @param x coordinate of block
     * @param y coordinate of block 0 <= y < 256
     * @param z coordinate of block
     * @param blockData new data of block
     */
    void ChangeDataNotify(int x, i_height y, int z, i_data blockData);

    /**
     * Change block data, but do not notify neighbor block that the block change
     * @param x coordinate of block
     * @param y coordinate of block 0 <= y < 256
     * @param z coordinate of block
     * @param blockData new data of block
     */
    void ChangeDataNoEvent(int x, i_height y, int z, i_data blockData);

    /**
     * Remove block at given coordinates, then perform on block drop
     * and replace it by block defined in database (by default: air block)
     * and notify all neighbor that the block change
     * @param x coordinate of block
     * @param y coordinate of block 0 <= y < 256
     * @param z coordinate of block
     */
    void BreakBlock(int x, i_height y, int z);

    /**
     * Remove block at coordinate, and replace it by block defined in database
     * (by default, air block)
     * and notify all neighbor that the block change
     * @param x coordinate of block
     * @param y coordinate of block 0 <= y < 256
     * @param z coordinate of block
     */
    void RemoveBlock(int x, i_height y, int z);

    /**
     * Add the block to update list, so Block::UpdateTick will be call
     * `waitTick` after the current tick.
     * @param x coordinate of block
     * @param y coordinate of block 0 <= y < 256
     * @param z coordinate of block
     * @param blockId current block id, so if block change, UpdateTick will not be call
     * @param waitTick number of tick to wait
     */
    void MarkBlockForUpdate(int x, i_height y, int z, i_block blockId, unsigned int waitTick = 1);


    /**
     * Call Block::NeighborChange on each block on give block's sides
     * @param x coordinate of block
     * @param y coordinate of block 0 <= y < 256
     * @param z coordinate of block
     * @param blockId blockId that has change
     */
    void NotifyNeighborsForBlockChange(int x, i_height y, int z, i_block blockId);

    /**
     * Call Block::NeighborChange on asked block
     * @param x coordinate of block
     * @param y coordinate of block 0 <= y < 256
     * @param z coordinate of block
     * @param neighborBlockId blockId that has change
     */
    void NotifyNeighborBlockChange(int x, i_height y, int z, i_block neighborBlockId);

    /**
     * Get the associated tile entity of block at given coordinate.
     * If there is no tile entity, then return nullptr
     * @param x coordinate of block
     * @param y coordinate of block 0 <= y < 256
     * @param z coordinate of block
     * @return return tile entity or nullptr
     */
    Block::TileEntity* GetTileEntity(int x, i_height y, int z);

    /**
     * Notify block that something change in it's associated tileEntity
     * finally call BlockScript::OnNotifyTileEntityStateChange
     * @param x coordinate of block
     * @param y coordinate of block 0 <= y < 256
     * @param z coordinate of block
     * @param action int defined in tileentity to say what happen (like for furnace start/end smelting)
     */
    void NotifyTileEntityStateChange(int x, i_height y, int z, int action);

    /**
     * Mark tile entity as updated, so player will be notified of the change
     * @param x coordinate of block
     * @param y coordinate of block 0 <= y < 256
     * @param z coordinate of block
     */
    void MarkForNetworkUpdateTileEntity(int x, i_height y, int z);



    /*************************************************************************
     * Blocks helper
     *************************************************************************/

    inline bool IsFullBlock(int x, i_height y, int z);
    bool IsNormalCube(int x, i_height y, int z);




    /*************************************************************************
     * Sound management
     *************************************************************************/

    /**
     * Send packet for playing sound to all player in range
     * @param x sound source coordinate
     * @param y sound source coordinate
     * @param z sound source coordinate
     * @param soundName name of sound
     * @param volume volume of sound
     * @param modifier modifier (pitch), default : 1.0f
     * @param distanceChunk distance in chunk where player cand listen sound
     */
    void PlaySound(double x, double y, double z, const std::wstring& soundName, float volume, float modifier, unsigned char distanceChunk);

    /**
     * Send packet for playing sound to all player in range
     * @param x sound source coordinate
     * @param y sound source coordinate
     * @param z sound source coordinate
     * @param soundName name of sound
     * @param volume volume of sound
     * @param modifier modifier (pitch), default : 63
     * @param distanceChunk distance in chunk where player cand listen sound
     */
    void PlaySound(double x, double y, double z, const std::wstring& soundName, float volume, char modifier, unsigned char distanceChunk);

    void PlaySoundOrParticleEffect(double x, i_height y, double z, int effectId, int data, bool disableRelativeVolume, unsigned char distanceChunk);

    void PlayBlockAction(int x, short y, int z, char type, char modifier, i_block blockId, char distanceChunk);




    /*************************************************************************
     * Network management
     *************************************************************************/

    /**
     * Notify all player of a change in game state, like when rain start or end etc...
     * @param reason
     * @param gameMode
     */
    void UpdateGameState(char reason, char gameMode);

    /**
     * Send a packet to all player currently in world
     * @param packet packet to send
     */
    void SendPacketToPlayerInWorld(const Network::NetworkPacket& packet) const;




    /*************************************************************************
     * Item management
     *************************************************************************/

    /**
     * Create new EntityItem and spawn it in world with random speed
     * containing itemstack
     * @param x spawn position
     * @param y spawn position
     * @param z spawn position
     * @param itemstack itemstack to drop, will be deleted when entity will be remove
     */
    void DropItemstackWithRandomDirection(double x, double y, double z, Inventory::ItemStack* itemstack);

    /**
     * Create new EntityItem and spawn it in world.
     * @param x spawn position
     * @param y spawn position
     * @param z spawn position
     * @param itemstack itemstack to drop, will be deleted when entity will be remove
     */
    void DropItemstack(double x, double y, double z, Inventory::ItemStack* itemstack);

    /**
     * Create new EntityItem and spawn it in world
     * @param pos position to spawn
     * @param itemstack itemstack to drop, will be deleted when entity will be remove
     */
    void DropItemstack(const Position& pos, Inventory::ItemStack* itemstack);




    /*************************************************************************
     * Collision management
     *************************************************************************/

    void GetBlockBoundingBoxInRange1(int x, int y, int z, std::vector<Util::AABB>& bbList);
    void GetBlockBoundingBoxInRange(int x, int y, int z, int range, int rangeHeight, std::vector<Util::AABB>& bbList);
    void GetBlockBoundingBoxInAABB(const Util::AABB& box, std::vector<Util::AABB>& bbList) const;

    void GetEntitiesBoundingBoxInAABB(const std::set<eEntityType> &type, int ignoreEntityId, const Util::AABB& boundingBox, std::vector<std::pair<int, Util::AABB>>& bbList);
    void GetEntitiesBoundingBoxInAABB(int ignoreEntityId, const Util::AABB& boundingBox, std::vector<std::pair<int, Util::AABB>>& bbList);
    void GetEntitiesBoundingBoxInAABBByEntityType(eEntityType type, int ignoreEntityId, const Util::AABB& boundingBox, std::vector<std::pair<int, Util::AABB>>& bbList);
    void GetEntitiesBoundingBoxInAABBByEntityFlag(int entityTypeFlag, int ignoreEntityId, const Util::AABB& boundingBox, std::vector<std::pair<int, Util::AABB>>& bbList);

    void GetEntitiesInRangeByEntityType(eEntityType type, int ignoreEntityId, const Position& center, int range, std::vector<Entity*>& outEntityList);
    void GetEntitiesInAABB(int ignoreEntityId, const Util::AABB& box, std::vector<Entity*>& outEntityList);
    void GetEntitiesInAABBByEntityType(eEntityType type, int ignoreEntityId, const Util::AABB& box, std::vector<Entity*>& outEntityList);
    void GetEntitiesInAABBByEntityFlag(int entityTypeFlag, int ignoreEntityId, const Util::AABB& box, std::vector<Entity*>& outEntityList);



    /*************************************************************************
     * Light management
     *************************************************************************/

    i_lightopacity GetBlockLightOpacity(int x, i_height y, int z);
    i_lightvalue GetRealLightValueAt(int x, i_height y, int z);


    /*************************************************************************
     * Redstone management
     *************************************************************************/

    i_powerlevel getBlockPower(int x, i_height y, int z, int direction);
    i_powerlevel computePowerLevelFromAroundBlock(int x, i_height y, int z);
    bool isBlockIndirectlyProvidingPowerTo(int x, i_height y, int z, int direction);
    i_powerlevel getIndirectPowerLevel(int x, i_height y, int z, int direction);
    bool isBlockIndirectlyGettingPowered(int x, i_height y, int z);
    i_powerlevel getMaxPowerFromBlockArround(int x, i_height y, int z);

    Scripting::BlockRedstoneTorchBurnoutMgr* GetRedstoneTorchBurnoutMgr() const;


private:

    /*************************************************************************
     * General world managmenet
     *************************************************************************/

    /**
     * Load level.dat
     */
    void load();




    /*************************************************************************
     * Entities managmenet
     *************************************************************************/

    void updateEntities();

    VirtualChunk* createVirtualChunk(int x, int z);

    VirtualSmallChunk* createVirtualSmallChunk(int x, int z);


    /**
     * Remove entity and delete them if it's specified (when not used anymore)
     * @param entity
     * @param deleteEntity if true, delete entity after removing it from world
     */
    void removeEntity(Entity* entity, bool deleteEntity);

    /**
     * Remove player from world
     * @param entity
     */
    void removePlayer(EntityPlayer* entity);

    /**
     * Mark entity to be deleted at the end of world update
     * @param entity entity to delete
     */
    void markEntityForDelete(Entity* entity);




    /*************************************************************************
     * Chunks managmenet
     *************************************************************************/

    Chunk* loadChunk(int x, int z);

    NBT::TagCompound* getChunkNbtData(int x, int z);

    void saveChunkNbtData(int x, int z, NBT::TagCompound* tag);

    bool isChunksExistInRange(int x, int z, int range) const;

    bool isChunksExist(int xmin, int zmin, int xmax, int zmax) const;

    bool isChunkExist(int chunkX, int chunkZ) const;

    i_height getMinHeightAndHeightMapAt(int x, int z, i_height& heightMap) const;

    i_height getMinHeightMapAt(int x, int z) const;




    /*************************************************************************
     * Weather and time managmenet
     *************************************************************************/

    void updateTime();

    /**
     * Load time and weather data from nbt in level.dat
     * Time of day...
     * Raining state/Rain time
     * Thunfer state/time
     * @param tagData data to load
     */
    void loadTimeAndWeather(NBT::TagCompound* tagData);

    void saveTimeAndWeather(NBT::TagCompound* tagData) const;




    /*************************************************************************
     * World parameters
     *************************************************************************/

    /**
     * Load spawn data from nbt in level.dat
     * @param tagData data to load
     */
    void loadSpawn(NBT::TagCompound* tagData);

    /**
     * Load gamemode and some data from world nbt data in level.dat
     * Seed
     * Hardcore mode
     * GameType
     * @param tagData data to load
     */
    void loadGameMode(NBT::TagCompound* tagData);

    void saveSpawn(NBT::TagCompound* tagData) const;

    void saveGameMode(NBT::TagCompound* tagData) const;





    /*************************************************************************
     * Block management
     *************************************************************************/

    /*************************************************************************
     * Blocks helper
     *************************************************************************/

    /*************************************************************************
     * Sound management
     *************************************************************************/

    /*************************************************************************
     * Network management
     *************************************************************************/

    /*************************************************************************
     * Item management
     *************************************************************************/

    /*************************************************************************
     * Collision management
     *************************************************************************/

    /*************************************************************************
     * Light management
     *************************************************************************/

    void updateAllLightTypes(int x, i_height y, int z);

    void updateLightByType(eLightType lightType, int x, i_height y, int z);

    void updateSkylightOnColumnt(int x, int z, i_height y1, i_height y2);

    i_lightvalue computeBlockLightValueUsingNeighbors(eLightType lightType, int x, i_height y, int z);

    i_lightvalue getLightValueAt(eLightType lightType, int x, i_height y, int z);

    void setLightValueAt(eLightType lightType, int x, i_height y, int z, i_lightvalue value);

    bool isBlockDirectlyLightedFromSky(int x, i_height y, int z);

    i_lightvalue recursiveGetRealLightValueAt(int x, i_height y, int z, bool firstCall);





    /*************************************************************************
     * Redstone management
     *************************************************************************/


private:

    /*************************************************************************
     * General world managmenet
     *************************************************************************/

    /// Name of the world
    std::string worldName;

    /// Name of the directory where world files are store
    std::string worldPath;

    /// True if world is performing UpateTick()
    bool updateInProgress;

    /*************************************************************************
     * Entities managmenet
     *************************************************************************/

    /// Map that store VirtualChunk, use macro CHUNK_KEY(x, z) to get a one
    std::unordered_map<long long, VirtualChunk*> virtualChunkMap;

    /// Map that store VirtualSmallChunk, use macro CHUNK_KEY(x, z) to get a one
    std::unordered_map<long long, VirtualSmallChunk*> virtualSmallChunkMap;

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

    /// Id of next entity
    int currentEntityId;


    /*************************************************************************
     * Chunks managmenet
     *************************************************************************/

    /// Manager for regions files
    RegionManager regionManager;

    /// Map that store Chunk, use macro CHUNK_KEY(x, z) to get a chunk
    std::unordered_map<long long, Chunk*> chunkMap;

    /*************************************************************************
     * Weather and time managmenet
     *************************************************************************/

    long long ageOfWorld;

    long long currentTime;

    int rainTime;

    bool raining;

    int thunderTime;

    bool thundering;

    bool weatherActivated;

    bool lockedTime;

    int lockedTimeValue;


    /*************************************************************************
     * World parameters
     *************************************************************************/

    /**
     * Distance of the player view in chunk, if 10, he will see 10 chunk in front of him/back etc... so 441 chunk
     * (10 + 1 + 10) * (10 + 1 + 10) = 441 chunk loaded around a player
     * (9 + 1 + 9) * (9 + 1 + 9) = 361 chunk loaded around a player
     * (8 + 1 + 8) * (8 + 1 + 8) = 289 chunk loaded around a player
     */
    int viewDistance;

    Position spawnPosition;

    bool hardcore;

    long long seed;

    int gameType;

    bool readOnly;

    bool enableUpdateChunk;

    /*************************************************************************
     * Block management
     *************************************************************************/

    /*************************************************************************
     * Blocks helper
     *************************************************************************/

    /*************************************************************************
     * Sound management
     *************************************************************************/

    /*************************************************************************
     * Network management
     *************************************************************************/

    /*************************************************************************
     * Item management
     *************************************************************************/

    /*************************************************************************
     * Collision management
     *************************************************************************/

    /*************************************************************************
     * Light management
     *************************************************************************/

    struct LightUpdateData
    {
        int x:6;
        int y:6;
        int z:6;
        unsigned level:4;
    };

    Util::BufferedRewindableQueue<struct LightUpdateData, 32768> updateLightQueue;

    i_lightvalue sunReduceValue;

    /*************************************************************************
     * Redstone management
     *************************************************************************/

    /// Redstone manager for burnout
    Scripting::BlockRedstoneTorchBurnoutMgr* redstoneTorchBurnoutMgr;

};

} /* namespace World */

#include "World.hxx"

#endif /* WORLD_H_ */
