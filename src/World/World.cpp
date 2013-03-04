#include "World.h"

#include <sstream>

#include "Chunk.h"
#include "Entity/EntityPlayer.h"
#include "Network/NetworkPacket.h"
#include "Network/OpcodeList.h"
#include "VirtualChunk.h"

namespace World
{

World::World() :
    viewDistance(10), currentEntityId(10), ageOfWorld(0), currentTime(0)
{
}

World::~World()
{
}

// Called each tick
void World::UpdateTick()
{
for (std::pair<long, Chunk*> chunk : chunkMap)
    {
        chunk.second->UpdateTick();
    }
for (std::pair<long, VirtualChunk*> chunk : virtualChunkMap)
    {
        chunk.second->UpdateTick();
    }
for (std::pair<long, VirtualChunk*> chunk : virtualChunkMap)
    {
        chunk.second->SendUpdate();
    }
for (std::pair<long, Chunk*> chunk : chunkMap)
    {
        chunk.second->SendUpdate();
    }

    UpdateTime();
}

void World::AddEntity(Entity* entity)
{
    entity->setWorld(this, currentEntityId++);
    VirtualChunk* virtualChunk = GetVirtualChunk(((int) entity->x) >> 8, ((int) entity->z) >> 8);
    virtualChunk->AddEntity(entity);
}

void World::AddPlayer(EntityPlayer* player)
{
    player->setWorld(this, currentEntityId++);
    playerList.insert(player);
    int chunkX = ((int) player->x) >> 4;
    int chunkZ = ((int) player->z) >> 4;
    int maxChunkX = chunkX + viewDistance;
    int maxChunkZ = chunkZ + viewDistance;

    for (int x = chunkX - viewDistance; x <= maxChunkX; x++)
        for (int z = chunkZ - viewDistance; z <= maxChunkZ; z++)
        {
            Chunk* chunk = GetChunk(x, z);
            chunk->AddPlayer(player);
            player->AddChunkToSend(x, z);
        }

    VirtualChunk* virtualChunk = GetVirtualChunk(((int) player->x) >> 8, ((int) player->z) >> 8);
    virtualChunk->AddPlayer(player);

    player->JoinWorld();

}

void World::RemoveEntity(Entity* entity)
{
    entity->setWorld(NULL, 0);
    VirtualChunk* virtualChunk = GetVirtualChunk(((int) entity->x) >> 8, ((int) entity->z) >> 8);
    virtualChunk->RemoveEntity(entity);
}

void World::RemovePlayer(EntityPlayer* player)
{
    playerList.erase(player);
    int chunkX = ((int) player->x) >> 4;
    int chunkZ = ((int) player->z) >> 4;
    int maxChunkX = chunkX + viewDistance;
    int maxChunkZ = chunkZ + viewDistance;

    for (int x = chunkX - viewDistance; x <= maxChunkX; x++)
        for (int z = chunkZ - viewDistance; z <= maxChunkZ; z++)
        {
            Chunk* chunk = GetChunk(x, z);
            chunk->RemovePlayer(player);
        }

    VirtualChunk* virtualChunk = GetVirtualChunk(((int) player->x) >> 7, ((int) player->z) >> 7);
    virtualChunk->RemovePlayer(player);
    player->setWorld(NULL, 0);
}

Chunk* World::LoadChunk(int x, int z)
{
    Chunk* chunk = new Chunk(x, z);
    chunk->Load();
    chunkMap[CHUNK_KEY(x,z)] = chunk;
    return chunk;
}

VirtualChunk* World::CreateVirtualChunk(int x, int z)
{
    VirtualChunk* vChunk = new VirtualChunk(x, z, this);
    return vChunk;
}

void World::SendPacketToPlayerInWorld(const Network::NetworkPacket& packet) const
{
for (EntityPlayer* plr : playerList)
    {
        plr->Send(packet);
    }
}

void World::Unload()
{
for (std::pair<long, Chunk*> chunk : chunkMap)
    {
        chunk.second->Unload();
        delete chunk.second;
    }
for (std::pair<long, VirtualChunk*> chunk : virtualChunkMap)
    {
        chunk.second->Unload();
        delete chunk.second;
    }
}

void World::RequestChunk(EntityPlayer* player, std::pair<int, int> chunkCoord)
{
    Chunk* chunk = GetChunk(chunkCoord.first, chunkCoord.second);
    const Network::NetworkPacket& packet = chunk->GetPacket();
    //packet.dump();
    player->Send(packet);
}

int World::getViewDistance()
{
    return viewDistance;
}

void World::UpdateTime()
{
    ageOfWorld++;
    currentTime++;
    if (currentTime % 20 == 0)
    {
        Network::NetworkPacket updateTimePacket(Network::OP_TIME_UPDATE);
        updateTimePacket << ageOfWorld << currentTime;
        SendPacketToPlayerInWorld(updateTimePacket);
    }
}

} /* namespace World */
