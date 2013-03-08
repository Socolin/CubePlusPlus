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

} /* namespace World */
