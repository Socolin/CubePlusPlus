/*
 * Entity.cpp
 *
 *  Created on: 24 déc. 2012
 *      Author: bertrand
 */

#include "Entity.h"

#include "Network/Opcode.h"
#include "Network/NetworkPacket.h"
#include "World/VirtualChunk.h"
#include "World/World.h"

namespace World
{

Entity::Entity(double x, double y, double z) :
    Position(x, y, z), world(0), entityId(0), yaw(0), pitch(0), hasMove(false), hasRotate(false), isMoving(false), stopMoving(false), motionX(0), motionY(0), motionZ(0)
    , networkX(((int) x) * 32)
    , networkY(((int) y) * 32)
    , networkZ(((int) z) * 32)
    , virtualChunkX(((int)x) >> 7)
    , virtualChunkZ(((int)z) >> 7)
    , chunkX(((int)x) >> 4)
    , chunkZ(((int)z) >> 4)
{

}

Entity::~Entity()
{
    // TODO Auto-generated destructor stub
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
}

void Entity::Teleport(double x, double y, double z, float yaw, float pitch)
{
    Relocate(x, y, z);
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

void Entity::moveToChunk(int newChunkX, int newChunkZ)
{
}

} /* namespace Network */
