#ifndef VIRTUALSMALLCHUNK_H_
#define VIRTUALSMALLCHUNK_H_

#include <set>
#include <vector>
#include "Entity/EntityConstants.h"
#include "Util/AABB.h"

namespace Network
{
class NetworkPacket;
}
// This class represent a chunk of 16x16 for storing entity
namespace World
{
class Position;
class Entity;
class EntityPlayer;
class World;
class VirtualSmallChunk
{
public:
    VirtualSmallChunk(int x, int z, World* world);
    virtual ~VirtualSmallChunk();

    // When spawning, not adding by moving
    void AddEntity(Entity* entity);
    void RemoveEntity(Entity* entity);
    void AddPlayer(EntityPlayer* player);
    void RemovePlayer(EntityPlayer* player);

    void SendPacketToAllNearPlayer(const Network::NetworkPacket& packet) const;
    void SendPacketToAllNearPlayer(const Network::NetworkPacket& packet, int distanceChunk) const;
    void SendPacketToPlayerInChunk(const Network::NetworkPacket& packet) const;

    void GetEntitiesBoundingBoxInAABB(int ignoreEntityId, const Util::AABB& box, std::vector<std::pair<int, Util::AABB> >& bbList);
    void GetEntitiesBoundingBoxInAABB(const std::set<eEntityType> &type, int ignoreEntityId, const Util::AABB& box, std::vector<std::pair<int, Util::AABB> >& bbList);
    void GetEntitiesBoundingBoxInAABBByEntityType(eEntityType type, int ignoreEntityId, const Util::AABB& box, std::vector<std::pair<int, Util::AABB> >& bbList);
    void GetEntitiesBoundingBoxInAABBByEntityFlag(int entityTypeFlag, int ignoreEntityId, const Util::AABB& box, std::vector<std::pair<int, Util::AABB> >& bbList);

    void GetEntitiesInRangeByEntityType(eEntityType type, int ignoreEntityId, const Position& center, int squaredRange, std::vector<Entity*>& outEntityList);
    void GetEntitiesInAABB(int ignoreEntityId, const Util::AABB& box, std::vector<Entity*>& outEntityList);
private:

    const int posX;
    const int posZ;
    World* world;
    std::set<Entity*> entityList;
    std::set<EntityPlayer*> playerList;
};

} /* namespace World */
#endif /* VIRTUALSMALLCHUNK_H_ */
