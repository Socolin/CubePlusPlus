#include "VirtualChunk.h"

#include "Entity/EntityPlayer.h"
#include "Network/NetworkPacket.h"
#include "World/World.h"

namespace World
{


VirtualChunk::VirtualChunk(int x, int z, World* world)
    : posX(x), posZ(z), world(world)
{
}

VirtualChunk::~VirtualChunk()
{
}

void VirtualChunk::AddEntity(Entity* entity)
{
    Network::NetworkPacket entityJoinPacket;
    entity->GetCreatePacket(entityJoinPacket);
    SendPacketToAllNearPlayer(entityJoinPacket);
    entityList.insert(entity);
}

void VirtualChunk::RemoveEntity(Entity* entity)
{
    entityList.erase(entity);
    Network::NetworkPacket entityDestroyPacket;
    entity->GetDestroyPacket(entityDestroyPacket);
    SendPacketToAllNearPlayer(entityDestroyPacket);
}

void VirtualChunk::AddPlayer(EntityPlayer* player)
{
    Network::NetworkPacket entityJoinPacket;
    for (int x = posX - 1; x <= posX + 1; x++)
        for (int z = posZ - 1; z <= posZ + 1; z++)
        {
            VirtualChunk* chunk = world->GetVirtualChunk(x, z);
            chunk->GetCreatePacketFromAllEntityInChunk(entityJoinPacket);
        }
    if (entityJoinPacket.getPacketSize())
        player->Send(entityJoinPacket);
    AddEntity(player);
    playerList.insert(player);
}

void VirtualChunk::RemovePlayer(EntityPlayer* player)
{
    Network::NetworkPacket entityDestroyPacket;
    for (int x = posX - 1; x <= posX + 1; x++)
        for (int z = posZ - 1; z <= posZ + 1; z++)
        {
            VirtualChunk* chunk = world->GetVirtualChunk(x, z);
            chunk->GetDestroyPacketFromAllEntityInChunk(entityDestroyPacket);
        }
    player->Send(entityDestroyPacket);
    RemoveEntity(player);
    playerList.erase(player);
}

void VirtualChunk::SendPacketToAllNearPlayer(const Network::NetworkPacket& packet) const
{
    for (int x = posX - 1; x <= posX + 1; x++)
        for (int z = posZ - 1; z <= posZ + 1; z++)
        {
            VirtualChunk* chunk = world->GetVirtualChunk(x, z);
            chunk->SendPacketToPlayerInChunk(packet);
        }
}

void VirtualChunk::UpdateTick()
{
    for (Entity* entity : entityList)
    {
        entity->UpdateTick();
    }
}

void VirtualChunk::SendUpdate()
{
    Network::NetworkPacket updatePacket;
    for (Entity* entity : entityList)
    {
        entity->GetUpdatePositionAndRotationPacket(updatePacket);
    }
    if (updatePacket.getPacketSize())
    {
        SendPacketToAllNearPlayer(updatePacket);
    }
}

void VirtualChunk::SendPacketToPlayerInChunk(const Network::NetworkPacket& packet) const
{
    for (EntityPlayer* plr : playerList)
    {
        plr->Send(packet);
    }
}

void VirtualChunk::GetCreatePacketFromAllEntityInChunk(Network::NetworkPacket& packet)
{
    for (Entity* entity : entityList)
    {
        entity->GetCreatePacket(packet);
    }
}

void VirtualChunk::GetDestroyPacketFromAllEntityInChunk(Network::NetworkPacket& packet)
{
    for (Entity* entity : entityList)
    {
        entity->GetDestroyPacket(packet);
    }
}

} /* namespace World */
