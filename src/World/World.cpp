#include "World.h"

#include <sstream>

#include "Chunk.h"
#include "Block/Block.h"
#include "Block/BlockList.h"
#include "Entity/EntityPlayer.h"
#include "Network/NetworkPacket.h"
#include "Network/OpcodeList.h"
#include "VirtualChunk.h"
#include "VirtualSmallChunk.h"

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
    VirtualSmallChunk *vChunk = GetVirtualSmallChunk(((int) entity->x) >> 4, ((int) entity->z) >> 4);
    vChunk->AddEntity(entity);
}

void World::AddPlayer(EntityPlayer* player)
{
    player->setWorld(this, currentEntityId++);
    playerList.insert(player);
    int chunkX = ((int) player->x) >> 4;
    int chunkZ = ((int) player->z) >> 4;

    VirtualSmallChunk *vChunk = GetVirtualSmallChunk(chunkX, chunkZ);
    vChunk->AddPlayer(player);

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
    VirtualSmallChunk *vChunk = GetVirtualSmallChunk(((int) entity->x) >> 4, ((int) entity->z) >> 4);
    vChunk->RemoveEntity(entity);
}

void World::RemovePlayer(EntityPlayer* player)
{
    playerList.erase(player);
    int chunkX = ((int) player->x) >> 4;
    int chunkZ = ((int) player->z) >> 4;
    int maxChunkX = chunkX + viewDistance;
    int maxChunkZ = chunkZ + viewDistance;

    VirtualSmallChunk *vChunk = GetVirtualSmallChunk(chunkX, chunkZ);
    vChunk->RemovePlayer(player);

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

void World::PlaceBlock(int x, i_height y, int z, i_block blockId, i_data blockData)
{
    Chunk* chunk = GetChunk(x >> 4, z >> 4);
    chunk->ChangeBlock(x & 0xf, y, z & 0xf, blockId, blockData);
    Block::Block* block = Block::BlockList::getBlock(blockId);
    if (block)
    {
        const Block::SoundBlock& sound = block->GetSound();
        Network::NetworkPacket soundPacket(Network::OP_NAMED_SOUND_EFFECT);
        soundPacket << sound.GetPlaceSound() << (x * 8) << (y * 8) << (z * 8)
                << sound.GetVolume() << (char)(sound.GetModifier() * 63.f);
        VirtualSmallChunk* vSmallChunk = GetVirtualSmallChunk(x >> 4, z >> 4);
        vSmallChunk->SendPacketToAllNearPlayer(soundPacket);
        if (block->UseTileEntity())
        {
            Block::TileEntity* tileEntity = block->CreateNewTileEntity();
            chunk->SetTileEntity(tileEntity ,x & 0xf, y, z & 0xf);
        }
    }
}

void World::PlaySound(double x, double y, double z, const std::wstring& soundName, float volume, char modifier, unsigned char distanceChunk)
{
    VirtualSmallChunk* vSmallChunk = GetVirtualSmallChunk(((int)x) >> 4, ((int)z) >> 4);
    Network::NetworkPacket soundPacket(Network::OP_NAMED_SOUND_EFFECT);
    soundPacket << soundName << (int)(x * 8) << (int)(y * 8) << (int)(z * 8) << volume << modifier;
    vSmallChunk->SendPacketToAllNearPlayer(soundPacket, distanceChunk);
}

void World::PlaySoundOrParticleEffect(double x, i_height y, double z, int effectId, int data, bool disableRelativeVolume, unsigned char distanceChunk)
{
    VirtualSmallChunk* vSmallChunk = GetVirtualSmallChunk(((int)x) >> 4, ((int)z) >> 4);
    Network::NetworkPacket soundPacket(Network::OP_SOUND_OR_PARTICLE_EFFECT);
    soundPacket << effectId << (int) x << y << (int) z << data << disableRelativeVolume;
    vSmallChunk->SendPacketToAllNearPlayer(soundPacket, distanceChunk);
}

void World::PlayBlockAction(int x, short y, int z, char type, char modifier, i_block blockId, char distanceChunk)
{
    VirtualSmallChunk* vSmallChunk = GetVirtualSmallChunk(((int)x) >> 4, ((int)z) >> 4);
    Network::NetworkPacket particlePacket(Network::OP_BLOCK_ACTION);
    particlePacket << x << y << z << type << modifier << blockId;
    vSmallChunk->SendPacketToAllNearPlayer(particlePacket, distanceChunk);
}

void World::GetBlockBoundingBoxInRange(int x, int y, int z, size_t range, size_t rangeHeight, std::vector<Util::AABB>& bbList)
{
    for (size_t blockX = x - range; blockX <= x + range; blockX++)
        for (size_t blockZ = z - range; blockZ <= z + range; blockZ++)
            for (size_t blockY = y - rangeHeight; blockY <= y + rangeHeight; blockY++)
            {
                s_block_data blockData = GetBlockIdAndData(blockX, blockY, blockZ);
                Block::Block* block = Block::BlockList::getBlock(blockData.blockId);
                if (block)
                {
                    block->GetBoundingBoxes(blockX, blockY, blockZ, blockData.blockData, bbList);
                }
            }
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

VirtualSmallChunk* World::CreateVirtualSmallChunk(int x, int z)
{
    VirtualSmallChunk* vChunk = new VirtualSmallChunk(x, z, this);
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
