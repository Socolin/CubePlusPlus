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
    playerList.erase(player);
    RemoveEntity(player);
    Network::NetworkPacket entityDestroyPacket;
    for (int x = posX - 1; x <= posX + 1; x++)
        for (int z = posZ - 1; z <= posZ + 1; z++)
        {
            VirtualChunk* chunk = world->GetVirtualChunk(x, z);
            chunk->GetDestroyPacketFromAllEntityInChunk(entityDestroyPacket);
        }
    player->Send(entityDestroyPacket);
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
        entity->GetSpecificUpdatePacket(updatePacket);
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


void VirtualChunk::AddPlayerByMoving(EntityPlayer* player, int prevChunkX, int prevChunkZ)
{
    if (prevChunkX != posX)
    {
        if (prevChunkZ != posZ) // Déplacement en diagonale
        {
            Network::NetworkPacket entityJoinPacket;
            int x = posX - (prevChunkX - posX);
            for (int z = posZ - 1; z <= posZ + 1;  z++)
            {
                VirtualChunk* chunk = world->GetVirtualChunk(x, z);
                chunk->GetCreatePacketFromAllEntityInChunk(entityJoinPacket);
            }
            int z = posZ - (prevChunkZ - posZ);
            for (int subX = posX - 1; subX <= posX + 1;  subX++)
            {
                if (subX == x)
                    continue;
                VirtualChunk* chunk = world->GetVirtualChunk(subX, z);
                chunk->GetCreatePacketFromAllEntityInChunk(entityJoinPacket);
            }
            if (entityJoinPacket.getPacketSize())
                player->Send(entityJoinPacket);
        }
        else // Déplacement sur l'axe X
        {
            Network::NetworkPacket entityJoinPacket;
            int x = posX - (prevChunkX - posX);
            for (int z = posZ - 1; z <= posZ + 1;  z++)
            {
                VirtualChunk* chunk = world->GetVirtualChunk(x, z);
                chunk->GetCreatePacketFromAllEntityInChunk(entityJoinPacket);
            }
            if (entityJoinPacket.getPacketSize())
                player->Send(entityJoinPacket);
        }
    }
    else if (prevChunkZ != posZ) // Déplacement sur l'axe Z
    {
        Network::NetworkPacket entityJoinPacket;
        int z = posZ - (prevChunkZ - posZ);
        for (int x = posX - 1; x <= posX + 1;  x++)
        {
            VirtualChunk* chunk = world->GetVirtualChunk(x, z);
            chunk->GetCreatePacketFromAllEntityInChunk(entityJoinPacket);
        }
        if (entityJoinPacket.getPacketSize())
            player->Send(entityJoinPacket);
    }
    else
    {
        std::cerr << "ERROR AddPlayerByMoving: " << posX << " " << posZ << " from " << prevChunkX << " " << prevChunkZ << std::endl;
    }
    AddEntityByMoving(player, prevChunkX, prevChunkZ);
    playerList.insert(player);
}

void VirtualChunk::RemovePlayerByMoving(EntityPlayer* player, int newChunkX, int newChunkZ)
{
    playerList.erase(player);
    RemoveEntityByMoving(player, newChunkX, newChunkZ);
    if (newChunkX != posX)
    {
        if (newChunkZ != posZ) // Déplacement en diagonale
        {
            Network::NetworkPacket entityJoinPacket;
            int x = posX - (newChunkX - posX);
            for (int z = posZ - 1; z <= posZ + 1;  z++)
            {
                VirtualChunk* chunk = world->GetVirtualChunk(x, z);
                chunk->GetDestroyPacketFromAllEntityInChunk(entityJoinPacket);
            }
            int z = posZ - (newChunkZ - posZ);
            for (int subX = posX - 1; subX <= posX + 1;  subX++)
            {
                if (subX == x)
                    continue;
                VirtualChunk* chunk = world->GetVirtualChunk(subX, z);
                chunk->GetDestroyPacketFromAllEntityInChunk(entityJoinPacket);
            }
            if (entityJoinPacket.getPacketSize())
                player->Send(entityJoinPacket);
        }
        else // Déplacement sur l'axe X
        {
            Network::NetworkPacket entityJoinPacket;
            int x = posX - (newChunkX - posX);
            for (int z = posZ - 1; z <= posZ + 1;  z++)
            {
                VirtualChunk* chunk = world->GetVirtualChunk(x, z);
                chunk->GetDestroyPacketFromAllEntityInChunk(entityJoinPacket);
            }
            if (entityJoinPacket.getPacketSize())
                player->Send(entityJoinPacket);
        }
    }
    else if (newChunkZ != posZ) // Déplacement sur l'axe Z
    {
        Network::NetworkPacket entityJoinPacket;
        int z = posZ - (newChunkZ - posZ);
        for (int x = posX - 1; x <= posX + 1;  x++)
        {
            VirtualChunk* chunk = world->GetVirtualChunk(x, z);
            chunk->GetDestroyPacketFromAllEntityInChunk(entityJoinPacket);
        }
        if (entityJoinPacket.getPacketSize())
            player->Send(entityJoinPacket);
    }
    else
    {
        std::cerr << "ERROR RemovePlayerByMoving: " << posX << " " << posZ << " to " << newChunkX << " " << newChunkZ << std::endl;
    }
}

void VirtualChunk::AddEntityByMoving(Entity* entity, int prevChunkX, int prevChunkZ)
{
    Network::NetworkPacket entityJoinPacket;
    entity->GetCreatePacket(entityJoinPacket);
    if (prevChunkX != posX)
    {
        if (prevChunkZ != posZ) // Déplacement en diagonale
        {
            int x = posX - (prevChunkX - posX);
            for (int z = posZ - 1; z <= posZ + 1;  z++)
            {
                VirtualChunk* chunk = world->GetVirtualChunk(x, z);
                chunk->SendPacketToPlayerInChunk(entityJoinPacket);
            }
            int z = posZ - (prevChunkZ - posZ);
            for (int subX = posX - 1; subX <= posX + 1;  subX++)
            {
                if (subX == x)
                    continue;
                VirtualChunk* chunk = world->GetVirtualChunk(subX, z);
                chunk->SendPacketToPlayerInChunk(entityJoinPacket);
            }
        }
        else // Déplacement sur l'axe X
        {
            int x = posX - (prevChunkX - posX);
            for (int z = posZ - 1; z <= posZ + 1;  z++)
            {
                VirtualChunk* chunk = world->GetVirtualChunk(x, z);
                chunk->SendPacketToPlayerInChunk(entityJoinPacket);
            }
        }
    }
    else if (prevChunkZ != posZ) // Déplacement sur l'axe Z
    {
        int z = posZ - (prevChunkZ - posZ);
        for (int x = posX - 1; x <= posX + 1;  x++)
        {
            VirtualChunk* chunk = world->GetVirtualChunk(x, z);
            chunk->SendPacketToPlayerInChunk(entityJoinPacket);
        }
    }
    else
    {
        std::cerr << "ERROR AddEntityByMoving: " << posX << " " << posZ << " from " << prevChunkX << " " << prevChunkZ << std::endl;
    }
    entityList.insert(entity);
}

void VirtualChunk::RemoveEntityByMoving(Entity* entity, int newChunkX, int newChunkZ)
{
    Network::NetworkPacket entityDestroyPacket;
    entity->GetDestroyPacket(entityDestroyPacket);
    if (newChunkX != posX)
    {
        if (newChunkZ != posZ) // Déplacement en diagonale
        {
            int x = posX - (newChunkX - posX);
            for (int z = posZ - 1; z <= posZ + 1;  z++)
            {
                VirtualChunk* chunk = world->GetVirtualChunk(x, z);
                chunk->SendPacketToPlayerInChunk(entityDestroyPacket);
            }
            int z = posZ - (newChunkZ - posZ);
            for (int subX = posX - 1; subX <= posX + 1;  subX++)
            {
                if (subX == x)
                    continue;
                VirtualChunk* chunk = world->GetVirtualChunk(subX, z);
                chunk->SendPacketToPlayerInChunk(entityDestroyPacket);
            }
        }
        else // Déplacement sur l'axe X
        {
            int x = posX - (newChunkX - posX);
            for (int z = posZ - 1; z <= posZ + 1;  z++)
            {
                VirtualChunk* chunk = world->GetVirtualChunk(x, z);
                chunk->SendPacketToPlayerInChunk(entityDestroyPacket);
            }
        }
    }
    else if (newChunkZ != posZ) // Déplacement sur l'axe Z
    {
        int z = posZ - (newChunkZ - posZ);
        for (int x = posX - 1; x <= posX + 1;  x++)
        {
            VirtualChunk* chunk = world->GetVirtualChunk(x, z);
            chunk->SendPacketToPlayerInChunk(entityDestroyPacket);
        }
    }
    else
    {
        std::cerr << "ERROR RemovePlayerByMoving: " << posX << " " << posZ << " to " << newChunkX << " " << newChunkZ << std::endl;
    }
    entityList.erase(entity);
}

void VirtualChunk::Unload()
{
}

} /* namespace World */
