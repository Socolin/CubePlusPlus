#include "VirtualSmallChunk.h"

#include "Entity/EntityPlayer.h"
#include "Network/NetworkPacket.h"
#include "World/World.h"

namespace World
{

VirtualSmallChunk::VirtualSmallChunk(int x, int z, World* world)
    : posX(x), posZ(z), world(world)
{
}

VirtualSmallChunk::~VirtualSmallChunk()
{
}
void VirtualSmallChunk::AddEntity(Entity* entity)
{
    entityList.insert(entity);
}

void VirtualSmallChunk::RemoveEntity(Entity* entity)
{
    entityList.erase(entity);
}

void VirtualSmallChunk::AddPlayer(EntityPlayer* player)
{
    AddEntity(player);
    playerList.insert(player);
}

void VirtualSmallChunk::RemovePlayer(EntityPlayer* player)
{
    playerList.erase(player);
    RemoveEntity(player);
}

void VirtualSmallChunk::SendPacketToAllNearPlayer(const Network::NetworkPacket& packet) const
{
    for (int x = posX - 1; x <= posX + 1; x++)
        for (int z = posZ - 1; z <= posZ + 1; z++)
        {
            VirtualSmallChunk* chunk = world->GetVirtualSmallChunkIfLoaded(x, z);
            if (chunk)
                chunk->SendPacketToPlayerInChunk(packet);
        }
}

void VirtualSmallChunk::SendPacketToAllNearPlayer(const Network::NetworkPacket& packet, int distanceChunk) const
{
    for (int x = posX - distanceChunk; x <= posX + distanceChunk; x++)
        for (int z = posZ - distanceChunk; z <= posZ + distanceChunk; z++)
        {
            VirtualSmallChunk* chunk = world->GetVirtualSmallChunkIfLoaded(x, z);
            if (chunk)
                chunk->SendPacketToPlayerInChunk(packet);
        }
}


void VirtualSmallChunk::SendPacketToPlayerInChunk(const Network::NetworkPacket& packet) const
{
for (EntityPlayer* plr : playerList)
    {
        plr->Send(packet);
    }
}

void VirtualSmallChunk::GetEntitiesBoundingBoxInAABB(const std::set<eEntityType>& type, int ignoreEntityId, const Util::AABB& box, std::vector<std::pair<int, Util::AABB> >& bbList)
{
    for (Entity* entity : entityList)
    {
        if (type.find(entity->getEntityType()) != type.end())
        {
            if (entity->getEntityId() != ignoreEntityId)
            {
                if (entity->CollideWith(box))
                {
                    bbList.push_back(std::make_pair(entity->getEntityId(), entity->GetBoundingBox()));
                }
            }
        }
    }
}

void VirtualSmallChunk::GetEntitiesBoundingBoxInAABBByEntityFlag(int entityTypeFlag, int ignoreEntityId, const Util::AABB& box, std::vector<std::pair<int, Util::AABB> >& bbList)
{
    for (Entity* entity : entityList)
    {
        if ((entity->GetEntityTypeFlag() & entityTypeFlag) != 0)
        {
            if (entity->getEntityId() != ignoreEntityId)
            {
                if (entity->CollideWith(box))
                {
                    bbList.push_back(std::make_pair(entity->getEntityId(), entity->GetBoundingBox()));
                }
            }
        }
    }
}


} /* namespace World */
