/*
 * Entity.cpp
 *
 *  Created on: 24 déc. 2012
 *      Author: bertrand
 */

#include "Entity.h"

#include "World/World.h"
#include "Network/Opcode.h"
#include "Network/NetworkPacket.h"

namespace World
{

Entity::Entity(double x, double y, double z) :
        Position(x, y, z), world(0), entityId(0), yaw(0), pitch(0), hasMove(false), hasRotate(false), oldX(x), oldY(y), oldZ(z)
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
    this->x = x;
    this->y = y;
    this->z = z;
    hasMove = true;
}

void Entity::Teleport(double x, double y, double z, float yaw, float pitch)
{
    Relocate(x, y, z);
}

void Entity::GetUpdatePositionAndRotationPacket(Network::NetworkPacket& packet)
{
    if (hasMove && hasRotate)
    {
        double dx = x - oldX;
        double dy = y - oldY;
        double dz = z - oldZ;
        if (dx > 4 || dy > 4 || dz > 4 || dx < -4 || dy < -4 || dz < -4)
        {
            packet << (unsigned char) Network::OP_ENTITY_TELEPORT << entityId << (int) x << (int) y << (int) z << (char) (yaw * 256.f / 360.f) << (char) (pitch * 256.f / 360.f);
            oldX = (int) x;
            oldY = (int) y;
            oldZ = (int) z;

            dx = x - (float) ((int) x);
            dy = y - (float) ((int) y);
            dz = z - (float) ((int) z);
        }
        packet << (unsigned char) Network::OP_ENTITY_LOOK_AND_RELATIVE_MOVE << entityId << (char) (dx / 32) << (char) (dy / 32) << (char) (dz / 32) << (char) (yaw * 256.f / 360.f) << (char) (pitch * 256.f / 360.f);
        oldX = x;
        oldY = y;
        oldZ = z;
        hasMove = false;
        hasRotate = false;
    }
    else if (hasMove)
    {
        double dx = x - oldX;
        double dy = y - oldY;
        double dz = z - oldZ;
        if (dx > 4 || dy > 4 || dz > 4 || dx < -4 || dy < -4 || dz < -4)
        {
            packet << (unsigned char) Network::OP_ENTITY_TELEPORT << entityId << (int) x << (int) y << (int) z << (char) (yaw * 256.f / 360.f) << (char) (pitch * 256.f / 360.f);
            oldX = (int) x;
            oldY = (int) y;
            oldZ = (int) z;

            dx = x - (float) ((int) x);
            dy = y - (float) ((int) y);
            dz = z - (float) ((int) z);
        }
        packet << (unsigned char) Network::OP_ENTITY_RELATIVE_MOVE << entityId << (char) (dx / 32) << (char) (dy / 32) << (char) (dz / 32);
        oldX = x;
        oldY = y;
        oldZ = z;
        hasMove = false;
    }
    else
    {
        packet << (unsigned char) Network::OP_ENTITY_LOOK << entityId << ((char) (yaw * 256.f / 360.f)) << ((char) (pitch * 256.f / 360.f));
        hasRotate = false;
    }
}

void Entity::GetDestroyPacket(Network::NetworkPacket& packet)
{
    packet << (unsigned char) Network::OP_DESTROY_ENTITY << entityId;
}

} /* namespace Network */
