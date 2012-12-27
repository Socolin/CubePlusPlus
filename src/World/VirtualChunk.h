#ifndef VIRTUALCHUNK_H_
#define VIRTUALCHUNK_H_

#include <set>

namespace Network
{
    class NetworkPacket;
}
namespace World
{

class Entity;
class EntityPlayer;
class World;
// This class represent a chunk of 128x128 for storing entity
class VirtualChunk
{
public:
    VirtualChunk(int x, int z, World* world);
    virtual ~VirtualChunk();

    // When spawning, not adding by moving
    void AddEntity(Entity* entity);
    void RemoveEntity(Entity* entity);
    void AddPlayer(EntityPlayer* player);
    void RemovePlayer(EntityPlayer* player);

    void UpdateTick();
    void SendUpdate();

    void SendPacketToAllNearPlayer(const Network::NetworkPacket& packet) const;
    void SendPacketToPlayerInChunk(const Network::NetworkPacket& packet) const;

    void GetCreatePacketFromAllEntityInChunk(Network::NetworkPacket& packet);
    void GetDestroyPacketFromAllEntityInChunk(Network::NetworkPacket& packet);
private:
    const int posX;
    const int posZ;
    World* world;
    std::set<Entity*> entityList;
    std::set<EntityPlayer*> playerList;
};

} /* namespace World */
#endif /* VIRTUALCHUNK_H_ */
