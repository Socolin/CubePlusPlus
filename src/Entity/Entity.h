/*
 * Entity.h
 *
 *  Created on: 24 déc. 2012
 *      Author: bertrand
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include "Position.h"

namespace Network
{
class NetworkPacket;
}

namespace World
{
class World;
class Entity: public Position
{
public:
    Entity(double x, double y, double z);
    virtual ~Entity();
    virtual void UpdateTick() = 0;
    void setWorld(World* world, int entityId);

    void Rotate(float yaw, float pitch);
    void MoveTo(double x, double y, double z);
    void Teleport(double x, double y, double z, float yaw, float pitch);
    void GetUpdatePositionAndRotationPacket(Network::NetworkPacket& packet);
    virtual void GetDestroyPacket(Network::NetworkPacket& packet);
    virtual void GetCreatePacket(Network::NetworkPacket& packet) = 0;
protected:
    World* world;
    int entityId;
    float yaw;
    float pitch;
    bool hasMove;
    bool hasRotate;
    double oldX;
    double oldY;
    double oldZ;
};

} /* namespace Network */
#endif /* ENTITY_H_ */
