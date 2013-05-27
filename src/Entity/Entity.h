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
#include "EntityConstants.h"

namespace Network
{
class NetworkPacket;
}

namespace World
{
class World;
class EntityPlayer;
class Entity: public Position
{
    enum eEntityFlags
    {
        ENTITY_FLAG_BURNING     = 0x1,
        ENTITY_FLAG_SNEAKING    = 0x2,
        ENTITY_FLAG_RIDING      = 0x4,
        ENTITY_FLAG_SPRINTING   = 0x8,
        ENTITY_FLAG_EATING      = 0x10,
        ENTITY_FLAG_INVISIBLE   = 0x20,
    };
public:
    Entity(eEntityType entityType, int entityTypeFlag, double x, double y, double z);
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
    bool CollideWith(const Util::AABB& box);
    Util::AABB GetBoundingBox() const;

    virtual void GetSpecificUpdatePacket(Network::NetworkPacket& packet) = 0;
    virtual void GetDestroyPacket(Network::NetworkPacket& packet);
    virtual void GetCreatePacket(Network::NetworkPacket& packet) = 0;
    virtual void moveToVirtualChunk(int newVirtualChunkX, int newVirtualChunkZ);
    virtual void moveToChunk(int newChunkX, int newChunkZ);
    virtual void Interact(EntityPlayer* player);

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

    eEntityType getEntityType() const
    {
        return entityType;
    }
    int GetEntityTypeFlag() const;

    bool isDead() const;
    void kill();
    int getEntityId() const;

    char GetFlag();
    void SetFlag(char flag);
    bool IsSneak();
    void SetSneak(bool sneak);
    bool IsSprinting();
    void SetSprinting(bool sprinting);

protected:
    bool PushOutOfBlock(double x, double y, double z);

    const eEntityType entityType;
    const int entityTypeFlag;

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

} /* namespace World */
#endif /* ENTITY_H_ */
