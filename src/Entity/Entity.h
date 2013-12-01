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
class LivingEntity;
class Entity: public Position
{
    enum eEntityFlags
    {
        ENTITY_FLAG_BURNING = 0x1,
        ENTITY_FLAG_SNEAKING = 0x2,
        ENTITY_FLAG_RIDING = 0x4,
        ENTITY_FLAG_SPRINTING = 0x8,
        ENTITY_FLAG_EATING = 0x10,
        ENTITY_FLAG_INVISIBLE = 0x20,
    };
public:
    Entity(eEntityType entityType, int entityTypeFlag, double x, double y, double z);
    virtual ~Entity();
    virtual void UpdateTick() = 0;

    /**
     * Get the unique entity Id
     * @return
     */
    int GetEntityId() const;

    /**
     * Called when an entity join a world, it define the world and the entityId
     * it initialize all special coordinate too, virtualChunk position, network position
     * chunk position etc..
     * @param world the world joined by the entity
     * @param entityId the unique id for the entity
     */
    void SetWorld(World* world, int entityId);

    /**
     * Rotate entity, and mark here to send update rotation to player around it
     * @param yaw the yaw rotation (around axes y)
     * @param pitch the pitch rotation ("up/down" rotation for head)
     */
    void Rotate(float yaw, float pitch);

    /**
     * Move entity to coordinate, it does not check collision etc..
     * This method update chunk coordinate and notify chunk when the entity leave the
     * chunk or enter in new chunk.
     * @param x new x coordinate
     * @param y new y coordinate
     * @param z new z coordinate
     */
    void MoveTo(double x, double y, double z);

    /**
     * Try to move the entity, check collision with block etc...
     * @param dx
     * @param dy
     * @param dz
     */
    void Move(double dx, double dy, double dz);

    /**
     * Notify that entity have not move
     */
    void StopMoving();

    /**
     * No implemented yet
     * Teleport the entity
     * @param x
     * @param y
     * @param z
     * @param yaw
     * @param pitch
     */
    void Teleport(double x, double y, double z, float yaw, float pitch);

    /**
     * Fill 'packet' with data to notify change to player around it
     * @param packet packet to fill
     */
    void GetUpdatePositionAndRotationPacket(Network::NetworkPacket& packet);

    /**
     * Define bounding box size of entity
     * @param width
     * @param height
     */
    void SetWidthHeight(double width, double height);

    /**
     * Test if this entity is in collision with 'box'
     * @param box
     * @return
     */
    bool CollideWith(const Util::AABB& box);

    /**
     * Get the entity bounding box.
     * @return
     */
    const Util::AABB& GetBoundingBox() const;

    /**
     * Fill packet with some specific data depending of entity
     * @param packet
     */
    virtual void GetSpecificUpdatePacket(Network::NetworkPacket& packet) = 0;

    /**
     * Fill packet with information to "destroy" entity for other client
     * @param packet
     */
    virtual void GetDestroyPacket(Network::NetworkPacket& packet);

    /**
     * Fill packet with data to add entity on client side
     * @param packet
     */
    virtual void GetCreatePacket(Network::NetworkPacket& packet) = 0;

    /**
     * Called when player use right click on entity
     * @param player player who clicked
     */
    virtual ItemUseResult Interact(EntityPlayer* player);

    /**
     * Called when the entity is attacked
     * @param attacker
     */
    virtual void Attack(LivingEntity* attacker, int& damage);

    /**
     * Load entity from nbt datas
     * @param tagNbtData
     * @return
     */
    virtual bool Load(NBT::TagCompound* tagNbtData);

    /**
     * Save entity into nbt data
     * @param tagNbtData
     * @return
     */
    virtual bool Save(NBT::TagCompound* tagNbtData);

    /**
     * When a player collide this entity
     * @param player
     */
    virtual void OnCollideWithPlayer(EntityPlayer* player);

    /**
     * Get the current world of the entity
     * @return
     */
    World* GetWorld() const;

    /**
     * Get pitch rotation (up/down)
     * @return
     */
    float GetPitch() const;

    /**
     * Get yaw rotation (around axe y)
     * @return
     */
    float GetYaw() const;


    eEntityType GetEntityType() const;

    int GetEntityTypeFlag() const;

    /**
     * Return true if entity is dead
     * @return
     */
    bool IsDead() const;
    /**
     * Kill an entity
     */
    virtual void Kill();

    virtual void DealDamage(int damage);

    char GetFlag();
    void SetFlag(char flag);
    bool IsSneak();
    void SetSneak(bool sneak);
    bool IsSprinting();
    void SetSprinting(bool sprinting);
    double GetMotionX() const;
    double GetMotionY() const;
    double GetMotionZ() const;

    void Jump();
    bool isOnGround() const;
    double GetFallingSpeedFactor() const;
    void SetFallingSpeedFactor(double fallingSpeed);
    EntityMetadataManager* GetMetadataManager();

    void Push(double dx, double dy, double dz);

protected:
    /**
     * Called when entity move to new VirtualChunk (128x128)
     * @param newVirtualChunkX
     * @param newVirtualChunkZ
     */
    virtual void moveToVirtualChunk(int newVirtualChunkX, int newVirtualChunkZ);

    /**
     * Called when entity move to new chunk (16x16)
     * @param newChunkX
     * @param newChunkZ
     */
    virtual void moveToChunk(int newChunkX, int newChunkZ);

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
    float fallDistance;
    short fire;
    short air;
    int dimension;
    bool invulnerable;
    int portalCooldown;

    // Motion vector
    double motionX;
    double motionY;
    double motionZ;

    double fallingSpeedFactor;

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

    // Bounding box for calcul or find entity in greater range...
    // storing it here, for reduce useless allocation
    Util::AABB tempBoundingBox;

    bool dead;

    EntityMetadataManager metadataManager;
};

} /* namespace World */
#endif /* ENTITY_H_ */
