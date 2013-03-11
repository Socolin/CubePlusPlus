/*
 * Entity.cpp
 *
 *  Created on: 24 déc. 2012
 *      Author: bertrand
 */

#include "Entity.h"

#include "Network/Opcode.h"
#include "Network/NetworkPacket.h"
#include "Util/FloatUtil.h"
#include "World/VirtualChunk.h"
#include "World/VirtualSmallChunk.h"
#include "World/World.h"

namespace World
{

Entity::Entity(double x, double y, double z) :
    Position(x, y, z), world(0), entityId(0), yaw(0), pitch(0), hasMove(false), hasRotate(false), isMoving(false), stopMoving(false), noclip(false),onGround(false), motionX(0), motionY(0), motionZ(0)
    , networkX(((int) x) * 32)
    , networkY(((int) y) * 32)
    , networkZ(((int) z) * 32)
    , virtualChunkX(((int)x) >> 7)
    , virtualChunkZ(((int)z) >> 7)
    , chunkX(((int)x) >> 4)
    , chunkZ(((int)z) >> 4)
    , boundingBox(x, y, z, 0.6, 1.8, 0.6)
{

}

Entity::~Entity()
{
}

void Entity::setWorld(World* world, int entityId)
{
    this->world = world;
    this->entityId = entityId;
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
    // TODO: if movedistance > 1 chunk tpback
    if (chunk == NULL)
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
    // TODO
}

void Entity::GetUpdatePositionAndRotationPacket(Network::NetworkPacket& packet)
{
    int newNetworkX = (int) (x * 32);
    int newNetworkY = (int) (y * 32);
    int newNetworkZ = (int) (z * 32);
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
                packet << (unsigned char) Network::OP_ENTITY_VELOCITY << entityId << (short)(motionX * 32000) << (short)(motionY * 32000) << (short)(motionZ * 32000);
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
                packet << (unsigned char) Network::OP_ENTITY_VELOCITY << entityId << (short)(motionX * 32000) << (short)(motionY * 32000) << (short)(motionZ * 32000);
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

bool Entity::PushOutOfBlock(double x, double y, double z)
{
    int blockX = floor(x);
    int blockY = floor(y);
    int blockZ = floor(z);

    double dx = x - (double)blockX;
    double dy = y - (double)blockY;
    double dz = z - (double)blockZ;

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

} /* namespace Network */
