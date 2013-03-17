#ifndef WORLD_H_
#define WORLD_H_

#include <unordered_map>
#include <string>
#include <set>

#include "Util/AABB.h"
#include "Chunk.h"
#include "Entity/EntityConstants.h"

#define CHUNK_KEY(X,Z) ((((long)X << 32) & 0xffffffff00000000)| ((long)Z & 0x00000000ffffffff))

namespace Network
{
class NetworkPacket;
}
namespace World
{

class Chunk;
class Entity;
class EntityPlayer;
class VirtualChunk;
class VirtualSmallChunk;

class World
{
public:
    World();
    virtual ~World();

    void UpdateTick();

    void AddEntity(Entity* entity);
    void AddPlayer(EntityPlayer* entity);

    void RemoveEntity(Entity* entity);
    void RemovePlayer(EntityPlayer* entity);

    inline Chunk* GetChunk(int x, int z);
    inline Chunk* GetChunkIfLoaded(int x, int z) const;

    inline VirtualChunk* GetVirtualChunk(int x, int z);
    inline VirtualChunk* GetVirtualChunkIfLoaded(int x, int z) const;

    inline VirtualSmallChunk* GetVirtualSmallChunk(int x, int z);
    inline VirtualSmallChunk* GetVirtualSmallChunkIfLoaded(int x, int z) const;

    inline i_block GetBlockId(int x, i_height y, int z);
    inline i_data GetBlockData(int x, i_height y, int z);

    inline bool IsFullBlock(int x, i_height y, int z);

    inline s_block_data GetBlockIdAndData(int x, i_height y, int z) const;

    void ChangeDataNoEvent(int x, i_height y, int z, i_data blockData);
    void ChangeBlockNoEvent(int x, i_height y, int z, i_block blockId, i_data blockData);
    void ChangeBlock(int x, i_height y, int z, i_block blockId, i_data blockData, bool playSound = true);
    void RemoveBlock(int x, i_height y, int z);
    void SendPacketToPlayerInWorld(const Network::NetworkPacket& packet) const;

    void MarkBlockForUpdate(int x, i_height y, int z, i_block blockId, unsigned int waitTick = 1);

    void Unload();
    void RequestChunk(EntityPlayer* player, std::pair<int, int>);

    int getViewDistance();

    void PlaySound(double x, double y, double z, const std::wstring& soundName, float volume, char modifier, unsigned char distanceChunk);
    void PlaySoundOrParticleEffect(double x, i_height y, double z, int effectId, int data, bool disableRelativeVolume, unsigned char distanceChunk);
    void PlayBlockAction(int x, short y, int z, char type, char modifier, i_block blockId, char distanceChunk);

    void DropItemstackWithRandomDirection(double x, double y, double z, const Inventory::ItemStack& itemstack);
    void GetBlockBoundingBoxInRange1(int x, int y, int z, std::vector<Util::AABB>& bbList);
    void GetBlockBoundingBoxInRange(int x, int y, int z, int range, int rangeHeight, std::vector<Util::AABB>& bbList);
    void GetBlockBoundingBoxInAABB(const Util::AABB& box, std::vector<Util::AABB>& bbList) const;

    void GetEntitiesBoundingBoxInAABB(const std::set<eEntityType> &type, int ignoreEntityId, const Util::AABB& boundingBox, std::vector<std::pair<int, Util::AABB>>& bbList);

    void MarkEntityAsDead(int entityId);

    Entity* GetEntityById(int target);

private:
    void NotifyNeighborBlockChange(int x, i_height y, int z);
    void UpdateTime();
    VirtualChunk* CreateVirtualChunk(int x, int z);
    VirtualSmallChunk* CreateVirtualSmallChunk(int x, int z);
    Chunk* LoadChunk(int x, int z);
    void MarkEntityForDelete(Entity* entity);
private:
    std::unordered_map<long, Chunk*> chunkMap;
    std::unordered_map<long, VirtualChunk*> virtualChunkMap;
    std::unordered_map<long, VirtualSmallChunk*> virtualSmallChunkMap;
    std::set<EntityPlayer*> playerList;
    int viewDistance; // In chunk
    int currentEntityId;
    long ageOfWorld;
    long currentTime;

    std::map<int, Entity*> entityById;
    std::set<int> deadEntity;
    std::vector<Entity*> entityToDelete;
};

} /* namespace World */

#include "World.hxx"

#endif /* WORLD_H_ */
