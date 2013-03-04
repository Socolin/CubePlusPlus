#ifndef WORLD_H_
#define WORLD_H_

#include <unordered_map>
#include <string>
#include <set>
#include "Chunk.h"

#define CHUNK_KEY(X,Z) ((((long)X << 32) & 0xffffffff00000000)| ((long)Z & 0x00000000ffffffff))

namespace Network
{
class NetworkPacket;
}
namespace World
{

class Chunk;
class Entity;
class EntityPlayer;
class VirtualChunk;

class World
{
public:
    World();
    virtual ~World();
    void UpdateTick();
    void AddEntity(Entity* entity);
    void AddPlayer(EntityPlayer* entity);
    void RemoveEntity(Entity* entity);
    void RemovePlayer(EntityPlayer* entity);
    Chunk* LoadChunk(int x, int z);
    VirtualChunk* CreateVirtualChunk(int x, int z);

    inline Chunk* GetChunk(int x, int z)
    {
        auto it = chunkMap.find(CHUNK_KEY(x,z));
        if (it == chunkMap.end())
        {
            Chunk* chunk = LoadChunk(x, z);
            chunkMap[CHUNK_KEY(x,z)] = chunk;
            return chunk;
        }
        return it->second;
    }

    inline Chunk* GetChunkIfLoaded(int x, int z)
    {
        auto it = chunkMap.find(CHUNK_KEY(x,z));
        if (it == chunkMap.end())
        {
            return NULL;
        }
        return it->second;
    }

    inline VirtualChunk* GetVirtualChunk(int x, int z)
    {
        auto it = virtualChunkMap.find(CHUNK_KEY(x,z));
        if (it == virtualChunkMap.end())
        {
            VirtualChunk* vChunk = CreateVirtualChunk(x, z);
            virtualChunkMap[CHUNK_KEY(x,z)] = vChunk;
            return vChunk;
        }
        return it->second;
    }

    inline int GetBlockId(int x, unsigned char y, int z)
    {
        Chunk* chunk = GetChunkIfLoaded(x >> 4, z >> 4);
        if (chunk == nullptr)
            return 0;
        return chunk->getBlockAt(x & 0xf, y, z & 0xf);
    }

    inline int GetBlockData(int x, unsigned char y, int z)
    {
        Chunk* chunk = GetChunkIfLoaded(x >> 4, z >> 4);
        if (chunk == nullptr)
            return 0;
        return chunk->getBlockAt(x & 0xf, y, z & 0xf);
    }
    void SendPacketToPlayerInWorld(const Network::NetworkPacket& packet) const;

    void Unload();
    void RequestChunk(EntityPlayer* player, std::pair<int, int>);
    int getViewDistance();
private:
    void UpdateTime();
private:
    std::unordered_map<long, Chunk*> chunkMap;
    std::unordered_map<long, VirtualChunk*> virtualChunkMap;
    std::set<EntityPlayer*> playerList;
    int viewDistance; // In chunk
    int currentEntityId;
    long ageOfWorld;
    long currentTime;
};

} /* namespace World */
#endif /* WORLD_H_ */
