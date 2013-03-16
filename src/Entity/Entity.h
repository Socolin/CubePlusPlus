/*
 * Entity.h
 *
 *  Created on: 24 déc. 2012
 *      Author: bertrand
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <vector>

#include "Position.h"
#include "Util/AABB.h"
#include "EntityMetadata/EntityMetadataManager.h"

namespace Network
{
class NetworkPacket;
}

namespace World
{
enum eEntityType
{
    ENTITY_TYPE_NONE,
    ENTITY_TYPE_PLAYER,
    ENTITY_TYPE_ITEM,
    ENTITY_TYPE_FALLINGBLOCK,
};

class World;
class Entity: public Position
{
public:
    Entity(eEntityType entityType, double x, double y, double z);
    virtual ~Entity();
    virtual void UpdateTick() = 0;
    void setWorld(World* world, int entityId);

    void Rotate(float yaw, float pitch);
    void MoveTo(double x, double y, double z);
    void Move(double dx, double dy, double dz);
    void StopMoving();
    void Teleport(double x, double y, double z, float yaw, float pitch);
    void GetUpdatePositionAndRotationPacket(Network::NetworkPacket& packet);
    void SetWidthHeight(double width, double height);

    virtual void GetSpecificUpdatePacket(Network::NetworkPacket& packet) = 0;
    virtual void GetDestroyPacket(Network::NetworkPacket& packet);
    virtual void GetCreatePacket(Network::NetworkPacket& packet) = 0;
    virtual void moveToVirtualChunk(int newVirtualChunkX, int newVirtualChunkZ);
    virtual void moveToChunk(int newChunkX, int newChunkZ);

    World* getWorld() const
    {
        return world;
    }

    float getPitch() const
    {
        return pitch;
    }

    void setPitch(float pitch)
    {
        this->pitch = pitch;
    }

    float getYaw() const
    {
        return yaw;
    }

    void setYaw(float yaw)
    {
        this->yaw = yaw;
    }

    int getEntityType() const
    {
        return entityType;
    }

    bool isDead() const;
    void kill();
    int getEntityId() const;

protected:
    bool PushOutOfBlock(double x, double y, double z);

    const int entityType;

    World* world;
    int entityId;
    float yaw;
    float pitch;
    bool hasMove;
    bool hasRotate;
    bool isMoving;
    bool stopMoving;
    bool noclip;
    bool onGround;

    // Motion vector
    double motionX;
    double motionY;
    double motionZ;

    // Position * 32
    int networkX;
    int networkY;
    int networkZ;

    // VirutalChunk coordinate where entity is
    int virtualChunkX;
    int virtualChunkZ;

    // Chunk coordinate where entity is
    int chunkX;
    int chunkZ;

    Util::AABB boundingBox;

    bool dead;

    EntityMetadataManager metadataManager;
};

} /* namespace Network */
#endif /* ENTITY_H_ */
