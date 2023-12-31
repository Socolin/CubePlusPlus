#include "Entity.h"

#include <NBTField/NBTField.h>

#include "Network/Opcode.h"
#include "Network/NetworkPacket.h"
#include "Util/FloatUtil.h"
#include "World/VirtualChunk.h"
#include "World/VirtualSmallChunk.h"
#include "World/World.h"

namespace World
{

Entity::Entity(eEntityType entityType, int entityTypeFlag, double x, double y, double z) :
    Position(x, y, z), entityType(entityType), entityTypeFlag(entityTypeFlag), world(nullptr), entityId(0)
    , yaw(0), pitch(0), hasMove(false), hasRotate(false), isMoving(false)
    , stopMoving(false), noclip(false),onGround(true)
    , fallDistance(0)
    , fire(0)
    , air(0)
    , dimension(0)
    , invulnerable(false)
    , portalCooldown(0)
    , motionX(0), motionY(0), motionZ(0), fallingSpeedFactor(1)
    , networkX((int)(x * 32.0))
    , networkY((int)(y * 32.0))
    , networkZ((int)(z * 32.0))
    , virtualChunkX(((int)x) >> 7)
    , virtualChunkZ(((int)z) >> 7)
    , chunkX(((int)x) >> 4)
    , chunkZ(((int)z) >> 4)
    , boundingBox(x, y, z, 0.6, 1.8, 0.6)
    , tempBoundingBox(x, y, z, 0.6, 1.8, 0.6)
    , dead(false)
{
    metadataManager.SetEntityMetadata(0, (char)0);
    metadataManager.SetEntityMetadata(1, (short)300);
}

Entity::~Entity()
{
}

void Entity::SetWorld(World* world, int entityId)
{
    this->world = world;
    this->entityId = entityId;
    networkX = (int)(x * 32.0);
    networkY = (int)(y * 32.0);
    networkZ = (int)(z * 32.0);
    virtualChunkX = ((int)x) >> 7;
    virtualChunkZ = ((int)z) >> 7;
    chunkX = ((int)x) >> 4;
    chunkZ = ((int)z) >> 4;
    hasMove = false;
    hasRotate = false;
}

void Entity::OnJoinWorld(World* /*world*/)
{
}

void Entity::Rotate(float yaw, float pitch)
{
    this->yaw = yaw;
    this->pitch = pitch;
    hasRotate = true;
}

void Entity::MoveTo(double x, double y, double z)
{
    int newChunkX = ((int) x) >> 4;
    int newChunkZ = ((int) z) >> 4;
    Chunk* chunk = world->GetChunkIfLoaded(newChunkX, newChunkZ);

    int chunkDx = newChunkX - chunkX;
    int chunkDz = newChunkZ - chunkZ;
    // TODO: if movedistance > 1 chunk tpback
    if (chunk == NULL || abs(chunkDx) > 1 || abs(chunkDz) > 1)
    {
        Teleport(this->x, this->y, this->z, this->yaw, this->pitch);
        return;
    }
    motionX = x - this->x;
    motionY = y - this->y;
    motionZ = z - this->z;
    this->x = x;
    this->y = y;
    this->z = z;
    hasMove = true;
    isMoving = true;
    int newVirtualChunkX = ((int) x) >> 7;
    int newVirtualChunkZ = ((int) z) >> 7;

    if (newVirtualChunkX != virtualChunkX || newVirtualChunkZ != virtualChunkZ)
    {
        moveToVirtualChunk(newVirtualChunkX, newVirtualChunkZ);
    }

    if (newChunkX != chunkX || newChunkZ != chunkZ)
    {
        moveToChunk(newChunkX, newChunkZ);
    }

    virtualChunkX = newVirtualChunkX;
    virtualChunkZ = newVirtualChunkZ;
    chunkX = newChunkX;
    chunkZ = newChunkZ;
    if (entityType == ENTITY_TYPE_PLAYER)
        boundingBox.SetPositionCenteredXZ(x, y, z);

    // TODO:
    // BlockCollision (example: pressureplate)
    // Fire collision
    // Lava Collision
}


void Entity::Move(double dx, double dy, double dz)
{
    if (noclip)
    {
        MoveTo(x + dx, y + dy, z + dz);
        boundingBox.Move(dx, dy, dz);
        return;
    }

    double oldDx = dx;
    double oldDy = dy;
    double oldDz = dz;

    std::vector<Util::AABB> bbList;
    world->GetBlockBoundingBoxInRange1(floor(x + dx), floor(y + dy), floor(z + dz), bbList);

    for (Util::AABB& box : bbList)
        dx = box.GetXOffsetWith(boundingBox, dx);

    boundingBox.MoveX(dx);

    for (Util::AABB& box : bbList)
        dy = box.GetYOffsetWith(boundingBox, dy);
    boundingBox.MoveY(dy);

    onGround = oldDy < 0 && dy != oldDy;

    for (Util::AABB& box : bbList)
        dz = box.GetZOffsetWith(boundingBox, dz);

    boundingBox.MoveZ(dz);

    if (0 != dx || 0 != dy || 0 != dz)
        MoveTo(x + dx, y + dy, z + dz);

    if (oldDx != dx)
        dx = 0;
    if (oldDy != dy)
        dy = 0;
    if (oldDz != dz)
        dz = 0;

    motionX = dx;
    motionY = dy;
    motionZ = dz;
}

void Entity::Teleport(double x, double y, double z, float yaw, float pitch)
{
    Relocate(x, y, z);

    int newChunkX = ((int) x) >> 4;
    int newChunkZ = ((int) z) >> 4;
    int newVirtualChunkX = ((int) x) >> 7;
    int newVirtualChunkZ = ((int) z) >> 7;

    if (newVirtualChunkX != virtualChunkX || newVirtualChunkZ != virtualChunkZ)
    {
        teleportToVirtualChunk(newVirtualChunkX, newVirtualChunkZ);
    }

    if (newChunkX != chunkX || newChunkZ != chunkZ)
    {
        teleportToChunk(newChunkX, newChunkZ);
    }

    virtualChunkX = newVirtualChunkX;
    virtualChunkZ = newVirtualChunkZ;
    chunkX = newChunkX;
    chunkZ = newChunkZ;
    if (entityType == ENTITY_TYPE_PLAYER)
        boundingBox.SetPositionCenteredXZ(x, y, z);

    onTeleport(x, y, z, yaw, pitch);
}

void Entity::GetUpdatePositionAndRotationPacket(Network::NetworkPacket& packet)
{
    int newNetworkX = (int) (x * 32.0);
    int newNetworkY = (int) (y * 32.0);
    int newNetworkZ = (int) (z * 32.0);
    if (hasMove && hasRotate)
    {
        int dx = newNetworkX - networkX;
        int dy = newNetworkY - networkY;
        int dz = newNetworkZ - networkZ;
        if (dx > 128 || dy > 128 || dz > 128 || dx < -128 || dy < -128 || dz < -128)
        {
            packet << (unsigned char) Network::OP_ENTITY_TELEPORT << entityId << newNetworkX << newNetworkY << newNetworkZ << (char) (yaw * 256.f / 360.f) << (char) (pitch * 256.f / 360.f);
            networkX = newNetworkX;
            networkY = newNetworkY;
            networkZ = newNetworkZ;
        }
        else
        {
            if (dx > 4 || dy > 4 || dz > 4 || dx < -4 || dy < -4 || dz < -4)
            {
                packet << (unsigned char) Network::OP_ENTITY_LOOK_AND_RELATIVE_MOVE << entityId << (char) dx << (char) dy << (char) dz << (char) (yaw * 256.f / 360.f) << (char) (pitch * 256.f / 360.f);
                packet << (unsigned char) Network::OP_ENTITY_HEAD_LOOK << entityId << ((char) (yaw * 256.f / 360.f));
                packet << (unsigned char) Network::OP_ENTITY_VELOCITY << entityId << (short)(motionX * 8000) << (short)(motionY * 8000) << (short)(motionZ * 8000);
                networkX = newNetworkX;
                networkY = newNetworkY;
                networkZ = newNetworkZ;
            }
        }
        hasMove = false;
        hasRotate = false;
    }
    else if (hasMove)
    {
        int dx = newNetworkX - networkX;
        int dy = newNetworkY - networkY;
        int dz = newNetworkZ - networkZ;
        if (dx > 128 || dy > 128 || dz > 128 || dx < -128 || dy < -128 || dz < -128)
        {
            packet << (unsigned char) Network::OP_ENTITY_TELEPORT << entityId << newNetworkX << newNetworkY << newNetworkZ << (char) (yaw * 256.f / 360.f) << (char) (pitch * 256.f / 360.f);
            networkX = newNetworkX;
            networkY = newNetworkY;
            networkZ = newNetworkZ;
        }
        else
        {
            if (dx > 4 || dy > 4 || dz > 4 || dx < -4 || dy < -4 || dz < -4)
            {
                packet << (unsigned char) Network::OP_ENTITY_RELATIVE_MOVE << entityId << (char) dx << (char) dy << (char) dz;
                packet << (unsigned char) Network::OP_ENTITY_VELOCITY << entityId << (short)(motionX * 8000) << (short)(motionY * 8000) << (short)(motionZ * 8000);
                networkX = newNetworkX;
                networkY = newNetworkY;
                networkZ = newNetworkZ;
            }
        }
        if (isMoving)
        {

        }
        hasMove = false;
    }
    else if (hasRotate)
    {
        packet << (unsigned char) Network::OP_ENTITY_LOOK << entityId << ((char) (yaw * 256.f / 360.f)) << ((char) (pitch * 256.f / 360.f));
        packet << (unsigned char) Network::OP_ENTITY_HEAD_LOOK << entityId << ((char) (yaw * 256.f / 360.f));
        packet << (unsigned char) Network::OP_ENTITY_VELOCITY << entityId << (short) 0 << (short) 0 << (short) 0;
        hasRotate = false;
    }
    if (stopMoving)
    {
        stopMoving = false;
        packet << (unsigned char) Network::OP_ENTITY_VELOCITY << entityId << (short) 0 << (short) 0 << (short) 0;
    }


    if (metadataManager.HasChanged())
    {
        metadataManager.ClearChange();
        packet << (unsigned char) Network::OP_ENTITY_METADATA
                << entityId;
        metadataManager.Write(packet);
    }
}

void Entity::StopMoving()
{
    isMoving = false;
    stopMoving = true;
}

void Entity::GetDestroyPacket(Network::NetworkPacket& packet)
{
    packet << (unsigned char) Network::OP_DESTROY_ENTITY << (char) 1 << entityId;
}

void Entity::moveToVirtualChunk(int newVirtualChunkX, int newVirtualChunkZ)
{
    VirtualChunk *oldVChunk = world->GetVirtualChunk(virtualChunkX, virtualChunkZ);
    oldVChunk->RemoveEntityByMoving(this, newVirtualChunkX, newVirtualChunkZ);
    VirtualChunk *vChunk = world->GetVirtualChunk(newVirtualChunkX, newVirtualChunkZ);
    vChunk->AddEntityByMoving(this, virtualChunkX, virtualChunkZ);
}

void Entity::SetWidthHeight(double width, double height)
{
    boundingBox.SetWidthHeight(width, height);
}

void Entity::moveToChunk(int newChunkX, int newChunkZ)
{
    VirtualSmallChunk*oldVChunk = world->GetVirtualSmallChunk(chunkX, chunkZ);
    oldVChunk->RemoveEntity(this);
    VirtualSmallChunk *vChunk = world->GetVirtualSmallChunk(newChunkX, newChunkZ);
    vChunk->AddEntity(this);
}

ItemUseResult Entity::Interact(EntityPlayer* /*player*/)
{
    return ItemUseResult{false, false, 0};
}

bool Entity::CollideWith(const Util::AABB& box)
{
    return boundingBox.DetectCollision(box);
}

const Util::AABB& Entity::GetBoundingBox() const
{
    return boundingBox;
}

bool Entity::IsSneak()
{
    return GetFlag() & ENTITY_FLAG_SNEAKING;
}

void Entity::SetSneak(bool sneak)
{
    if (sneak)
    {
        SetFlag(GetFlag() | ENTITY_FLAG_SNEAKING);
    }
    else
    {
        SetFlag(GetFlag() & !ENTITY_FLAG_SNEAKING);
    }
}

bool Entity::IsSprinting()
{
    return GetFlag() & ENTITY_FLAG_SPRINTING;
}

void Entity::SetSprinting(bool sprinting)
{
    if (sprinting)
    {
        SetFlag(GetFlag() | ENTITY_FLAG_SPRINTING);
    }
    else
    {
        SetFlag(GetFlag() & !ENTITY_FLAG_SPRINTING);
    }
}

char Entity::GetFlag()
{
    return metadataManager.GetCharEntityMetadata(0);
}

void Entity::SetFlag(char flag)
{
    metadataManager.SetEntityMetadata(0, flag);
}

bool Entity::Load(NBT::TagCompound* tagNbtData)
{
    assert(tagNbtData != nullptr);
    NBT::TagList* tagPos = tagNbtData->GetTagAs<NBT::TagList>("Pos");
    if (!tagPos)
        return false;

    tagPos->FillVariablesWithList<NBT::TagDouble, double>({&x, &y, &z});

    NBT::TagList* tagMotion = tagNbtData->GetTagAs<NBT::TagList>("Motion");
    if (!tagMotion)
        return false;

    tagMotion->FillVariablesWithList<NBT::TagDouble, double>({&motionX, &motionY, &motionZ});

    NBT::TagList* tagRotation = tagNbtData->GetTagAs<NBT::TagList>("Rotation");
    if (!tagRotation)
        return false;

    tagRotation->FillVariablesWithList<NBT::TagFloat, float>({&yaw, &pitch});


    fallDistance = tagNbtData->GetFloat("FallDistance", 0);
    fire = tagNbtData->GetShort("Fire", -20);
    air = tagNbtData->GetShort("Air", 300);
    onGround = tagNbtData->GetBool("OnGround", true);
    dimension = tagNbtData->GetInt("Dimension", 0);
    invulnerable = tagNbtData->GetBool("Invulnerable", false);
    portalCooldown = tagNbtData->GetInt("PortalCooldown", 0);

    networkX = (int)(x * 32.0);
    networkY = (int)(y * 32.0);
    networkZ = (int)(z * 32.0);
    virtualChunkX = ((int)x) >> 7;
    virtualChunkZ = ((int)z) >> 7;
    chunkX = ((int)x) >> 4;
    chunkZ = ((int)z) >> 4;
    boundingBox.SetPosition(x, y, z);
    boundingBox.SetWidthHeightDepth(0.6, 1.8, 0.6);
    tempBoundingBox.SetPosition(x, y, z);
    tempBoundingBox.SetWidthHeightDepth(0.6, 1.8, 0.6);
    return true;
}

bool Entity::Save(NBT::TagCompound* tagNbtData)
{
    using namespace NBT;
    tagNbtData->AddTag(new TagList("Pos", {x, y, z}));
    tagNbtData->AddTag(new TagList("Motion", {motionX, motionY, motionZ}));
    tagNbtData->AddTag(new TagList("Rotation", {yaw, pitch}));
    tagNbtData->AddTag(new TagFloat("FallDistance", fallDistance));
    tagNbtData->AddTag(new TagShort("Fire", fire));
    tagNbtData->AddTag(new TagShort("Air", air));
    tagNbtData->AddTag(new TagByte("OnGround", onGround));
    tagNbtData->AddTag(new TagInt("Dimension", dimension));
    tagNbtData->AddTag(new TagByte("Invulnerable", invulnerable));
    tagNbtData->AddTag(new TagInt("PortalCooldown", portalCooldown));
    return true;
}

void Entity::OnCollideWithPlayer(EntityPlayer* /*player*/)
{
}

void Entity::Attack(LivingEntity* /*attacker*/, int& /*damage*/)
{
}

void Entity::DealDamage(int /*damage*/)
{
}

void Entity::Jump()
{
    motionY = 0.42;
}

void Entity::Push(double dx, double dy, double dz)
{
    motionX = dx;
    motionY = dy;
    motionZ = dz;
}

void Entity::teleportToVirtualChunk(int newVirtualChunkX, int newVirtualChunkZ)
{
    VirtualChunk *oldVChunk = world->GetVirtualChunk(virtualChunkX, virtualChunkZ);
    oldVChunk->RemoveEntity(this);
    VirtualChunk *vChunk = world->GetVirtualChunk(newVirtualChunkX, newVirtualChunkZ);
    vChunk->AddEntity(this);
}

void Entity::teleportToChunk(int newChunkX, int newChunkZ)
{
    VirtualSmallChunk*oldVChunk = world->GetVirtualSmallChunk(chunkX, chunkZ);
    oldVChunk->RemoveEntity(this);
    VirtualSmallChunk *vChunk = world->GetVirtualSmallChunk(newChunkX, newChunkZ);
    vChunk->AddEntity(this);
}

void Entity::onTeleport(double /*x*/, double /*y*/, double /*z*/, float /*yaw*/, float /*pitch*/)
{
}

bool Entity::pushOutOfBlock(double x, double y, double z)
{
    int blockX = floor(x);
    int blockY = floor(y);
    int blockZ = floor(z);

    bool collision = false;
    std::vector<Util::AABB> bbList;
    world->GetBlockBoundingBoxInAABB(boundingBox, bbList);
    for (Util::AABB& box : bbList)
    {
        if (boundingBox.DetectCollision(box))
        {
            collision = true;
            break;
        }
    }
    if (!collision && !world->IsFullBlock(blockX, blockY, blockZ))
    {
        return false;
    }
    else
    {
        double dx = x - (double)blockX;
        double dy = y - (double)blockY;
        double dz = z - (double)blockZ;

        bool blockWestFull = world->IsFullBlock(blockX - 1, blockY, blockZ);
        bool blockEastFull = world->IsFullBlock(blockX + 1, blockY, blockZ);
        bool blockTopFull = world->IsFullBlock(blockX, blockY + 1, blockZ);
        bool blockNorthFull = world->IsFullBlock(blockX, blockY, blockZ - 1);
        bool blockSouthFull = world->IsFullBlock(blockX, blockY, blockZ + 1);
        char direction = 3;
        double lowerDistance = 9999.0;

        if (!blockWestFull && dx < lowerDistance)
        {
            lowerDistance = dx;
            direction = 0;
        }

        if (!blockEastFull && 1.0 - dx < lowerDistance)
        {
            lowerDistance = 1.0 - dx;
            direction = 1;
        }

        if (!blockTopFull && 1.0 - dy < lowerDistance)
        {
            lowerDistance = 1.0 - dy;
            direction = 3;
        }

        if (!blockNorthFull && dz < lowerDistance)
        {
            lowerDistance = dz;
            direction = 4;
        }

        if (!blockSouthFull && 1.0 - dz < lowerDistance)
        {
            lowerDistance = 1.0 - dz;
            direction = 5;
        }

        float randomSpeed = Util::randFloat() * 0.2F + 0.1F;

        if (direction == 0)
        {
            motionX = -randomSpeed;
        }
        else if (direction == 1)
        {
            motionX = randomSpeed;
        }
        else if (direction == 3)
        {
            motionY = randomSpeed;
        }
        else if (direction == 4)
        {
            motionZ = -randomSpeed;
        }
        else if (direction == 5)
        {
            motionZ = randomSpeed;
        }

        return true;
    }

    return true;
}

bool Entity::IsDead() const
{
    return dead;
}

void Entity::Kill()
{
    dead = true;
    world->MarkEntityAsDead(entityId);
}

int Entity::GetEntityId() const
{
    return entityId;
}

World* Entity::GetWorld() const
{
    return world;
}

float Entity::GetPitch() const
{
    return pitch;
}

float Entity::GetYaw() const
{
    return yaw;
}

eEntityType Entity::GetEntityType() const
{
    return entityType;
}

int Entity::GetEntityTypeFlag() const
{
    return entityTypeFlag;
}

double Entity::GetMotionX() const
{
    return motionX;
}

double Entity::GetMotionY() const
{
    return motionY;
}

double Entity::GetMotionZ() const
{
    return motionZ;
}

bool Entity::IsOnGround() const
{
    return onGround;
}

double Entity::GetFallingSpeedFactor() const
{
    return fallingSpeedFactor;
}

void Entity::SetFallingSpeedFactor(double fallingSpeed)
{
    this->fallingSpeedFactor = fallingSpeed;
}

EntityMetadataManager* Entity::GetMetadataManager()
{
    return &metadataManager;
}

} /* namespace World */
