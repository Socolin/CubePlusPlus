#include "World.h"

#include <sstream>

#include "Chunk.h"
#include "Entity/EntityPlayer.h"
#include "VirtualChunk.h"

namespace World
{

World::World() :
        viewDistance(10), currentEntityId(1)
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
}

void World::AddEntity(Entity* entity)
{
    entity->setWorld(this, currentEntityId++);
    VirtualChunk* virtualChunk = GetVirtualChunk(((int) entity->x) >> 8, ((int) entity->z) >> 8);
    virtualChunk->AddEntity(entity);
}

void World::AddPlayer(EntityPlayer* entity)
{
    entity->setWorld(this, currentEntityId++);
    playerList.insert(entity);
    int chunkX = ((int) entity->x) >> 4;
    int chunkZ = ((int) entity->z) >> 4;
    int maxChunkX = chunkX + viewDistance;
    int maxChunkZ = chunkZ + viewDistance;

    for (int x = chunkX - viewDistance; x <= maxChunkX; x++)
        for (int z = chunkZ - viewDistance; z <= maxChunkZ; z++)
        {
            Chunk* chunk = GetChunk(x, z);
            chunk->AddRefCount();
            entity->AddChunkToSend(x, z);
        }

    VirtualChunk* virtualChunk = GetVirtualChunk(((int) entity->x) >> 8, ((int) entity->z) >> 8);
    virtualChunk->AddPlayer(entity);

    entity->JoinWorld();

}

void World::RemoveEntity(Entity* entity)
{
    entity->setWorld(NULL, 0);
    VirtualChunk* virtualChunk = GetVirtualChunk(((int) entity->x) >> 8, ((int) entity->z) >> 8);
    virtualChunk->RemoveEntity(entity);
}

void World::RemovePlayer(EntityPlayer* entity)
{
    entity->setWorld(NULL, 0);
    playerList.erase(entity);
    int chunkX = ((int) entity->x) >> 4;
    int chunkZ = ((int) entity->z) >> 4;
    int maxChunkX = chunkX + viewDistance;
    int maxChunkZ = chunkZ + viewDistance;

    for (int x = chunkX - viewDistance; x <= maxChunkX; x++)
        for (int z = chunkZ - viewDistance; z <= maxChunkZ; z++)
        {
            Chunk* chunk = GetChunk(x, z);
            chunk->RemoveRefCount();
        }

    VirtualChunk* virtualChunk = GetVirtualChunk(((int) entity->x) >> 8, ((int) entity->z) >> 8);
    virtualChunk->RemovePlayer(entity);
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

} /* namespace World */
