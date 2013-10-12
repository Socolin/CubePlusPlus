#include "World.h"

#include <cmath>
#include <sstream>
#include <algorithm>
#include <cppnbt.h>

#include "Chunk.h"
#include "Block/Block.h"
#include "Block/BlockConstants.h"
#include "Block/BlockList.h"
#include "Inventory/ItemStack.h"
#include "Entity/EntityPlayer.h"
#include "Entity/Object/EntityItem.h"
#include "Logging/Logger.h"
#include "Network/NetworkPacket.h"
#include "Network/OpcodeList.h"
#include "Region.h"
#include "Util/FloatUtil.h"
#include "Util/types.h"
#include "VirtualChunk.h"
#include "VirtualSmallChunk.h"
#include "Config/Config.h"

namespace World
{

World::World(const std::string& worldName)
    : worldName(worldName)
    , regionManager(worldName + "/")
    , updateInProgress(false)
    , viewDistance(10)
    , currentEntityId(10)
    , sunReduceValue(0)
    , ageOfWorld(0)
    , currentTime(0)
    , rainTime(0)
    , raining(false)
    , thunderTime(false)
    , thundering(false)
    , hardcore(false)
    , seed(0)
    , gameType(0)
    , weatherActivated(false)
{
    redstoneTorchBurnoutMgr = new Scripting::BlockRedstoneTorchBurnoutMgr();
    Config::Config::getConfig().lookupValue("server.world.read-only", readOnly);
    Config::Config::getConfig().lookupValue("server.world.time.locked", lockedTime);
    Config::Config::getConfig().lookupValue("server.world.time.locked-value", lockedTimeValue);
    Config::Config::getConfig().lookupValue("server.world.time.locked-value", lockedTimeValue);
    Config::Config::getConfig().lookupValue("server.world.chunk.update-tick", enableUpdateChunk);
    load();
}

World::~World()
{
    delete redstoneTorchBurnoutMgr;

    for (auto playerItr : playerList)
    {
        entityToDelete.push_back(playerItr);
    }
    for (size_t i = 0; i < entityToDelete.size(); i++)
    {
        delete entityToDelete[i];
    }
}


// Called each tick
void World::UpdateTick()
{
    updateInProgress = true;
    for (std::pair<long long, Chunk*> chunk : chunkMap)
    {
        chunk.second->UpdateTick();
    }
    for (std::pair<long long, VirtualChunk*> chunk : virtualChunkMap)
    {
        chunk.second->UpdateTick();
    }
    for (std::pair<long long, VirtualChunk*> chunk : virtualChunkMap)
    {
        chunk.second->SendUpdate();
    }
    for (std::pair<long long, Chunk*> chunk : chunkMap)
    {
        chunk.second->SendUpdate();
    }

    redstoneTorchBurnoutMgr->UpdateTick();
    updateInProgress = false;
    UpdateTime();
}

void World::AddEntity(Entity* entity)
{
    entity->SetWorld(this, currentEntityId++);
    VirtualChunk* virtualChunk = GetVirtualChunk(((int) entity->x) >> 7, ((int) entity->z) >> 7);
    virtualChunk->AddEntity(entity);
    VirtualSmallChunk *vChunk = GetVirtualSmallChunk(((int) entity->x) >> 4, ((int) entity->z) >> 4);
    vChunk->AddEntity(entity);
    entityById[entity->GetEntityId()] = entity;
}

void World::AddPlayer(EntityPlayer* player)
{
    player->SetWorld(this, currentEntityId++);
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

    VirtualChunk* virtualChunk = GetVirtualChunk(((int) player->x) >> 7, ((int) player->z) >> 7);
    virtualChunk->AddPlayer(player);
    player->OnJoinWorld(this);
    entityById[player->GetEntityId()] = player;
}

void World::RemoveEntity(Entity* entity)
{
    VirtualChunk* virtualChunk = GetVirtualChunk(((int) entity->x) >> 7, ((int) entity->z) >> 7);
    virtualChunk->RemoveEntity(entity);
    VirtualSmallChunk *vChunk = GetVirtualSmallChunk(((int) entity->x) >> 4, ((int) entity->z) >> 4);
    vChunk->RemoveEntity(entity);
    entityById[entity->GetEntityId()] = nullptr;
    entity->SetWorld(nullptr, 0);
}

void World::RemovePlayer(EntityPlayer* player)
{
    if (updateInProgress)
    {
        MarkPlayerForRemove(player);
        return;
    }
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
    player->SetWorld(nullptr, 0);
    entityById[player->GetEntityId()] = nullptr;
}

void World::ChangeBlockNoEvent(int x, i_height y, int z, i_block blockId, i_data blockData)
{
    Chunk* chunk = GetChunk(x >> 4, z >> 4);
    chunk->ChangeBlock(x & 0xf, y, z & 0xf, blockId, blockData);
    updateAllLightTypes(x, y, z);
}

void World::ChangeDataNoEvent(int x, i_height y, int z, i_data blockData)
{
    Chunk* chunk = GetChunk(x >> 4, z >> 4);
    chunk->ChangeData(x & 0xf, y, z & 0xf, blockData);
}

void World::ChangeDataNotify(int x, i_height y, int z, i_data blockData)
{
    Chunk* chunk = GetChunk(x >> 4, z >> 4);
    chunk->ChangeData(x & 0xf, y, z & 0xf, blockData);

    i_block blockId = chunk->getBlockAt(x & 0xf, y, z & 0xf);
    FOR_EACH_SIDE_XYZ(x, y, z, blockSide)
        NotifyNeighborBlockChange(blockSideX, blockSideY, blockSideZ, blockId);
    END_FOR_EACH_SIDE
}


void World::ChangeBlock(int x, i_height y, int z, i_block blockId, i_data blockData, bool playSound)
{
    Chunk* chunk = GetChunk(x >> 4, z >> 4);
    chunk->ChangeBlock(x & 0xf, y, z & 0xf, blockId, blockData);

    const Block::Block* block = Block::BlockList::getBlock(blockId);
    if (block)
    {
        if (playSound)
        {
            const Block::SoundBlock& sound = block->GetSound();
            Network::NetworkPacket soundPacket(Network::OP_NAMED_SOUND_EFFECT);
            soundPacket << sound.GetPlaceSound() << (x * 8) << (y * 8) << (z * 8)
                    << sound.GetVolume() << (char)(sound.GetModifier() * 63.f);
            VirtualSmallChunk* vSmallChunk = GetVirtualSmallChunk(x >> 4, z >> 4);
            vSmallChunk->SendPacketToAllNearPlayer(soundPacket);
        }
    }

    FOR_EACH_SIDE_XYZ(x, y, z, blockSide)
        NotifyNeighborBlockChange(blockSideX, blockSideY, blockSideZ, blockId);
    END_FOR_EACH_SIDE

    updateAllLightTypes(x, y, z);
}

void World::RemoveBlock(int x, i_height y, int z)
{
    Chunk* chunk = GetChunk(x >> 4, z >> 4);
    i_block blockId = chunk->getBlockAt(x & 0xf, y, z & 0xf);
    if (blockId > 0)
    {
        chunk->ChangeBlock(x & 0xf, y, z & 0xf, 0, 0);

        FOR_EACH_SIDE_XYZ(x, y, z, blockSide)
            NotifyNeighborBlockChange(blockSideX, blockSideY, blockSideZ, 0);
        END_FOR_EACH_SIDE
    }
    updateAllLightTypes(x, y, z);
}

void World::NotifyNeighborBlockChange(int x, i_height y, int z, i_block neighborBlockId)
{
    i_block blockId = GetBlockId(x, y, z);
    const Block::Block* block = Block::BlockList::getBlock(blockId);
    if (block)
    {
        block->NeighborChange(this, x, y, z, neighborBlockId);
    }
}

void World::NotifyTileEntityStateChange(int x, i_height y, int z, int action)
{
    i_block blockId = GetBlockId(x, y, z);
    const Block::Block* block = Block::BlockList::getBlock(blockId);
    if (block)
    {
        block->NotifyTileEntityStateChange(this, x, y, z, action);
    }
}

void World::PlaySound(double x, double y, double z, const std::wstring& soundName, float volume, float modifier, unsigned char distanceChunk)
{
    PlaySound(x, y, z, soundName, volume, char(63 * modifier), distanceChunk);
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

void World::UpdateGameState(char reason, char gameMode)
{
    Network::NetworkPacket packet(Network::OP_CHANGE_GAME_STATE);
    packet << reason << gameMode;
    SendPacketToPlayerInWorld(packet);
}

void World::GetBlockBoundingBoxInRange(int x, int y, int z, int range, int rangeHeight, std::vector<Util::AABB>& bbList)
{
    for (int blockX = x - range; blockX <= x + range; blockX++)
        for (int blockZ = z - range; blockZ <= z + range; blockZ++)
            for (int blockY = y - rangeHeight; blockY <= y + rangeHeight; blockY++)
            {
                s_block_data blockData = GetBlockIdAndData(blockX, blockY, blockZ);
                if (blockData.blockId > 0)
                {
                    const Block::Block* block = Block::BlockList::getBlock(blockData.blockId);
                    if (block)
                    {
                        block->GetBoundingBoxes(blockX, blockY, blockZ, blockData.blockData, bbList);
                    }
                }
            }
}

void World::GetBlockBoundingBoxInAABB(const Util::AABB& box, std::vector<Util::AABB>& bbList) const
{
    int minX = floor(box.getX());
    int minY = floor(box.getY());
    int minZ = floor(box.getZ());
    int maxX = floor(box.getMaxX());
    int maxY = floor(box.getMaxY());
    int maxZ = floor(box.getMaxZ());
    for (int blockX = minX; blockX <= maxX; blockX++)
        for (int blockZ = minZ; blockZ <= maxZ; blockZ++)
            for (int blockY = minY; blockY <= maxY; blockY++)
            {
                s_block_data blockData = GetBlockIdAndData(blockX, blockY, blockZ);
                if (blockData.blockId > 0)
                {
                    const Block::Block* block = Block::BlockList::getBlock(blockData.blockId);
                    if (block)
                    {
                        block->GetBoundingBoxes(blockX, blockY, blockZ, blockData.blockData, bbList);
                    }
                }
            }
}


void World::GetBlockBoundingBoxInRange1(int x, int y, int z, std::vector<Util::AABB>& bbList)
{
    for (int blockX = x - 1; blockX <= x + 1; blockX++)
        for (int blockZ = z - 1; blockZ <= z + 1; blockZ++)
            for (int blockY = y - 1; blockY <= y + 1; blockY++)
            {
                s_block_data blockData = GetBlockIdAndData(blockX, blockY, blockZ);
                if (blockData.blockId > 0)
                {
                    const Block::Block* block = Block::BlockList::getBlock(blockData.blockId);
                    if (block)
                    {
                        block->GetBoundingBoxes(blockX, blockY, blockZ, blockData.blockData, bbList);
                    }
                }
            }
}

void World::DropItemstackWithRandomDirection(double x, double y, double z, Inventory::ItemStack* itemstack)
{
    if (itemstack == nullptr)
        return;
    const Inventory::Item* item = itemstack->getItem();
    if (item != nullptr)
    {
        float randomDistance = Util::randFloat() * 0.1;
        float randomAngle = Util::randFloat() * M_PI * 2.0;

        double motionX = -sin(randomAngle) * randomDistance;
        double motionZ = cos(randomAngle) * randomDistance;
        double motionY = 0.20000000298023224;

        EntityItem* item = new EntityItem(x, y, z, itemstack, motionX, motionY, motionZ);
        AddEntity(item);
    }
}

void World::DropItemstack(double x, double y, double z, Inventory::ItemStack* itemstack)
{
    if (itemstack == nullptr)
        return;
    const Inventory::Item* item = itemstack->getItem();
    if (item != nullptr)
    {
        EntityItem* item = new EntityItem(x, y, z, itemstack, 0, 0, 0);
        AddEntity(item);
    }
}

void World::DropItemstack(const Position& pos, Inventory::ItemStack* itemstack)
{
    DropItemstack(pos.x, pos.y, pos.z, itemstack);
}

void World::GetEntitiesBoundingBoxInAABB(int ignoreEntityId, const Util::AABB& box, std::vector<std::pair<int, Util::AABB>>& bbList)
{
    int minX = floor(box.getX()) - 2;
    int minZ = floor(box.getZ()) - 2;
    int maxX = floor(box.getMaxX()) + 2;
    int maxZ = floor(box.getMaxZ()) + 2;
    minX >>= 4;
    minZ >>= 4;
    maxX >>= 4;
    maxZ >>= 4;
    for (int chunkX = minX; chunkX <= maxX; chunkX++)
        for (int chunkZ = minZ; chunkZ <= maxZ; chunkZ++)
            {
                VirtualSmallChunk* vSmallChunk = GetVirtualSmallChunk(chunkX, chunkZ);
                vSmallChunk->GetEntitiesBoundingBoxInAABB(ignoreEntityId, box, bbList);
            }
}


void World::GetEntitiesBoundingBoxInAABB(const std::set<eEntityType> &type, int ignoreEntityId, const Util::AABB& box, std::vector<std::pair<int, Util::AABB>>& bbList)
{
    int minX = floor(box.getX()) - 2;
    int minZ = floor(box.getZ()) - 2;
    int maxX = floor(box.getMaxX()) + 2;
    int maxZ = floor(box.getMaxZ()) + 2;
    minX >>= 4;
    minZ >>= 4;
    maxX >>= 4;
    maxZ >>= 4;
    for (int chunkX = minX; chunkX <= maxX; chunkX++)
        for (int chunkZ = minZ; chunkZ <= maxZ; chunkZ++)
            {
                VirtualSmallChunk* vSmallChunk = GetVirtualSmallChunk(chunkX, chunkZ);
                vSmallChunk->GetEntitiesBoundingBoxInAABB(type, ignoreEntityId, box, bbList);
            }
}

void World::GetEntitiesBoundingBoxInAABBByEntityType(eEntityType type, int ignoreEntityId, const Util::AABB& box, std::vector<std::pair<int, Util::AABB>>& bbList)
{
    int minX = floor(box.getX()) - 2;
    int minZ = floor(box.getZ()) - 2;
    int maxX = floor(box.getMaxX()) + 2;
    int maxZ = floor(box.getMaxZ()) + 2;
    minX >>= 4;
    minZ >>= 4;
    maxX >>= 4;
    maxZ >>= 4;
    for (int chunkX = minX; chunkX <= maxX; chunkX++)
        for (int chunkZ = minZ; chunkZ <= maxZ; chunkZ++)
            {
                VirtualSmallChunk* vSmallChunk = GetVirtualSmallChunk(chunkX, chunkZ);
                vSmallChunk->GetEntitiesBoundingBoxInAABBByEntityType(type, ignoreEntityId, box, bbList);
            }
}
void World::GetEntitiesBoundingBoxInAABBByEntityFlag(int entityTypeFlag, int ignoreEntityId, const Util::AABB& box, std::vector<std::pair<int, Util::AABB>>& bbList)
{
    int minX = floor(box.getX()) - 2;
    int minZ = floor(box.getZ()) - 2;
    int maxX = floor(box.getMaxX()) + 2;
    int maxZ = floor(box.getMaxZ()) + 2;
    minX >>= 4;
    minZ >>= 4;
    maxX >>= 4;
    maxZ >>= 4;
    for (int chunkX = minX; chunkX <= maxX; chunkX++)
        for (int chunkZ = minZ; chunkZ <= maxZ; chunkZ++)
            {
                VirtualSmallChunk* vSmallChunk = GetVirtualSmallChunk(chunkX, chunkZ);
                vSmallChunk->GetEntitiesBoundingBoxInAABBByEntityFlag(entityTypeFlag, ignoreEntityId, box, bbList);
            }
}
void World::GetEntitiesInRangeByEntityType(eEntityType type, int ignoreEntityId, const Position& center, int range, std::vector<Entity*>& outEntityList)
{
    int minX = floor(center.x - range);
    int minZ = floor(center.z - range);
    int maxX = floor(center.x + range);
    int maxZ = floor(center.z + range);
    minX >>= 4;
    minZ >>= 4;
    maxX >>= 4;
    maxZ >>= 4;
    int distanceSquared = range * range;
    for (int chunkX = minX; chunkX <= maxX; chunkX++)
        for (int chunkZ = minZ; chunkZ <= maxZ; chunkZ++)
            {
                VirtualSmallChunk* vSmallChunk = GetVirtualSmallChunk(chunkX, chunkZ);
                vSmallChunk->GetEntitiesInRangeByEntityType(type, ignoreEntityId, center, distanceSquared, outEntityList);
            }
}

void World::GetEntitiesInAABB(int ignoreEntityId, const Util::AABB& box, std::vector<Entity*>& outEntityList)
{
    int minX = floor(box.getX()) - 1;
    int minZ = floor(box.getZ()) - 1;
    int maxX = floor(box.getMaxX()) + 1;
    int maxZ = floor(box.getMaxZ()) + 1;
    minX >>= 4;
    minZ >>= 4;
    maxX >>= 4;
    maxZ >>= 4;
    for (int chunkX = minX; chunkX <= maxX; chunkX++)
        for (int chunkZ = minZ; chunkZ <= maxZ; chunkZ++)
            {
                VirtualSmallChunk* vSmallChunk = GetVirtualSmallChunk(chunkX, chunkZ);
                vSmallChunk->GetEntitiesInAABB(ignoreEntityId, box, outEntityList);
            }
}

void World::GetEntitiesInAABBByEntityType(eEntityType type, int ignoreEntityId, const Util::AABB& box, std::vector<Entity*>& outEntityList)
{
    int minX = floor(box.getX()) - 1;
    int minZ = floor(box.getZ()) - 1;
    int maxX = floor(box.getMaxX()) + 1;
    int maxZ = floor(box.getMaxZ()) + 1;
    minX >>= 4;
    minZ >>= 4;
    maxX >>= 4;
    maxZ >>= 4;
    for (int chunkX = minX; chunkX <= maxX; chunkX++)
        for (int chunkZ = minZ; chunkZ <= maxZ; chunkZ++)
            {
                VirtualSmallChunk* vSmallChunk = GetVirtualSmallChunk(chunkX, chunkZ);
                vSmallChunk->GetEntitiesInAABBByEntityType(type, ignoreEntityId, box, outEntityList);
            }
}

void World::GetEntitiesInAABBByEntityFlag(int entityTypeFlag, int ignoreEntityId, const Util::AABB& box, std::vector<Entity*>& outEntityList)
{
    int minX = floor(box.getX()) - 1;
    int minZ = floor(box.getZ()) - 1;
    int maxX = floor(box.getMaxX()) + 1;
    int maxZ = floor(box.getMaxZ()) + 1;
    minX >>= 4;
    minZ >>= 4;
    maxX >>= 4;
    maxZ >>= 4;
    for (int chunkX = minX; chunkX <= maxX; chunkX++)
        for (int chunkZ = minZ; chunkZ <= maxZ; chunkZ++)
            {
                VirtualSmallChunk* vSmallChunk = GetVirtualSmallChunk(chunkX, chunkZ);
                vSmallChunk->GetEntitiesInAABBByEntityFlag(entityTypeFlag, ignoreEntityId, box, outEntityList);
            }
}

void World::MarkEntityAsDead(int entityId)
{
    deadEntity.insert(entityId);
}

Chunk* World::LoadChunk(int x, int z)
{
    Chunk* chunk = new Chunk(x, z, this);
    chunk->Load();
    chunkMap[CHUNK_KEY(x,z)] = chunk;
    return chunk;
}
nbt::NbtBuffer* World::GetChunkNbtData(int x, int z)
{
    Region* region = regionManager.GetRegion(x >> 5, z >> 5);
    if (region)
    {
        nbt::NbtBuffer* nbtData = region->GetNbtChunkData(x & 0x1f, z & 0x1f);
        return nbtData;
    }
    return nullptr;
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
    for (std::pair<long long, Chunk*> chunk : chunkMap)
    {
        chunk.second->Unload();
        delete chunk.second;
    }
    chunkMap.clear();
    for (std::pair<long long, VirtualChunk*> chunk : virtualChunkMap)
    {
        chunk.second->Unload();
        delete chunk.second;
    }
    virtualChunkMap.clear();
    for (std::pair<long long, VirtualSmallChunk*> chunk : virtualSmallChunkMap)
    {
        delete chunk.second;
    }
    virtualSmallChunkMap.clear();
}

void World::RequestChunk(EntityPlayer* player, int x, int z)
{
    Chunk* chunk = GetChunk(x, z);
    const Network::NetworkPacket& packet = chunk->GetPacket();

    //packet.dump();
    player->Send(packet);

    Network::NetworkPacket tileEntitiesPacket;
    chunk->GetTileEntityPacket(tileEntitiesPacket);
    player->Send(tileEntitiesPacket);
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
        if (lockedTime)
        {
            currentTime = lockedTimeValue;
        }
        Network::NetworkPacket updateTimePacket(Network::OP_TIME_UPDATE);
        updateTimePacket << ageOfWorld << currentTime;
        SendPacketToPlayerInWorld(updateTimePacket);
    }

    for (int deadEntityId : deadEntity)
    {
        Entity* entity = entityById[deadEntityId];
        if (entity)
        {
            if (entity->GetEntityType() == ENTITY_TYPE_PLAYER)
            {
                EntityPlayer* player = dynamic_cast<EntityPlayer*>(entity);
                if (player)
                {
                    RemovePlayer(player);
                    continue;
                }
            }
            RemoveEntity(entity);
            MarkEntityForDelete(entity);
        }
    }
    deadEntity.clear();
    for (size_t i = 0; i < entityToDelete.size(); i++)
    {
        delete entityToDelete[i];
    }
    entityToDelete.clear();

    for (EntityPlayer* player: playerToRemove)
        RemovePlayer(player);
    playerToRemove.clear();

    if (weatherActivated)
    {
        rainTime--;
        if (rainTime <= 0)
        {
            raining = !raining;
            UpdateGameState(raining ? 1 : 2, 0);

            if (raining)
            {
                rainTime = (rand() % 12000) + 12000;
            }
            else
            {
                rainTime = (rand() % 168000) + 12000;
            }
        }
    }
}

void World::MarkBlockForUpdate(int x, i_height y, int z, i_block blockId, unsigned int waitTick)
{
    Chunk* chunk = GetChunk(x >> 4, z >> 4);
    chunk->MarkForUpdate(x & 0xf, y, z & 0xf, blockId, waitTick);
}

Entity* World::GetEntityById(int target)
{
    return entityById[target];
}

void World::MarkEntityForDelete(Entity* entity)
{
    entityToDelete.push_back(entity);
}

/*
 *
 */
bool World::isChunksExistInRange(int x, i_height y, int z, int range)
{
    return isChunksExist(x - range, (i_height) std::max(0, y - range), z - range, x + range, (i_height) std::min(255, y + range), z + range);

}

bool World::isChunksExist(int xmin, i_height /*ymin*/, int zmin, int xmax, i_height /*ymax*/, int zmax)
{
    xmin >>= 4;
    zmin >>= 4;
    xmax >>= 4;
    zmax >>= 4;

    for (int chunkX = xmin; chunkX <= xmax; ++chunkX)
    {
        for (int chunkZ = zmin; chunkZ <= zmax; ++chunkZ)
        {
            if (!isChunkExist(chunkX, chunkZ))
            {
                return false;
            }
        }
    }

    return true;
}

bool World::isChunkExist(int chunkX, int chunkZ)
{
    Chunk* chunk = GetChunkIfLoaded(chunkX, chunkZ);
    return chunk != nullptr;
}

/* **************************************************************************
 *  Light relative's functions
 *  *************************************************************************
 */
void World::updateAllLightTypes(int x, i_height y, int z)
{
    updateLightByType(LIGHTTYPE_SKY, x, y, z);
    updateLightByType(LIGHTTYPE_BLOCK, x, y, z);
}

void World::updateLightByType(eLightType lightType, int x, i_height y, int z)
{
    if (!isChunksExistInRange(x, y, z, 17))
        return;

    updateLightQueue.reset();

    i_lightvalue oldBlockLightValue = getLightValueAt(lightType, x, y, z);
    i_lightvalue newBlockLightValue = computeBlockLightValueUsingNeighbors(lightType, x, y, z);

    if (newBlockLightValue > oldBlockLightValue)
    {
        updateLightQueue.push({0,0,0,0});
    }
    else if (newBlockLightValue < oldBlockLightValue)
    {
        updateLightQueue.push({0,0,0,oldBlockLightValue});
        while (!updateLightQueue.empty())
        {
            struct LightUpdateData blockToUpdate = updateLightQueue.pop();
            int blockToUpdateX = blockToUpdate.x + x;
            int blockToUpdateY = blockToUpdate.y + y;
            int blockToUpdateZ = blockToUpdate.z + z;

            i_lightvalue blockToUpdateLightValue = blockToUpdate.l;
            i_lightvalue blockToUpdateOldLightValue = getLightValueAt(lightType, blockToUpdateX, blockToUpdateY, blockToUpdateZ);

            if (blockToUpdateOldLightValue == blockToUpdateLightValue)
            {
                setLightValueAt(lightType, blockToUpdateX, blockToUpdateY, blockToUpdateZ, 0);
                if (blockToUpdateLightValue > 0)
                {
                    int dx = abs(blockToUpdateX - x);
                    int dy = abs(blockToUpdateY - y);
                    int dz = abs(blockToUpdateZ - z);

                    if (dx + dy + dz < 17) // http://en.wikipedia.org/wiki/Taxicab_geometry
                    {
                        FOR_EACH_SIDE_YZX(blockToUpdateX, blockToUpdateY, blockToUpdateZ, blockSide)
                            const Block::Block* block = Block::BlockList::getBlock(GetBlockId(blockSideX, blockSideY, blockSideZ));

                            i_lightvalue blockSideLightValue = 1;
                            if (block != nullptr)
                                blockSideLightValue = std::max(i_lightvalue(1), block->GetLightOpacity());
                            i_lightvalue blockSideOldLightValue = getLightValueAt(lightType, blockSideX, blockSideY, blockSideZ);

                            // Si le block qu'on viens d'update avait peut etre une influance sur la lumière du block calculé
                            // ex: si le block qu'on update est le 13, il influance la lumière du 12 car il était entre lui et la source de lumière
                            // 15 14 13 12 11
                            // T  15 14[13]12
                            // 15 14 13 12 11
                            // 14 13 12 11 10
                            if (blockSideOldLightValue == blockToUpdateLightValue - blockSideLightValue)
                            {
                                updateLightQueue.push({ blockSideX - x, blockSideY - y, blockSideZ - z, i_lightvalue(blockToUpdateLightValue - blockSideLightValue) });
                            }
                        }
                    }
                }
            }
        }
        updateLightQueue.rewind();
    }
    while (!updateLightQueue.empty())
    {
        struct LightUpdateData blockToUpdate = updateLightQueue.pop();
        int blockToUpdateX = blockToUpdate.x + x;
        int blockToUpdateY = blockToUpdate.y + y;
        int blockToUpdateZ = blockToUpdate.z + z;

        i_lightvalue blockToUpdateOldLightValue = getLightValueAt(lightType, blockToUpdateX, blockToUpdateY, blockToUpdateZ);
        i_lightvalue blockToUpdateNewLightValue = computeBlockLightValueUsingNeighbors(lightType, blockToUpdateX, blockToUpdateY, blockToUpdateZ);

        if (blockToUpdateNewLightValue != blockToUpdateOldLightValue)
        {
            setLightValueAt(lightType, blockToUpdateX, blockToUpdateY, blockToUpdateZ, blockToUpdateNewLightValue);
            if (blockToUpdateNewLightValue > blockToUpdateOldLightValue)
            {
                int dx = abs(blockToUpdateX - x);
                int dy = abs(blockToUpdateY - y);
                int dz = abs(blockToUpdateZ - z);

                if (dx + dy + dz < 17 && updateLightQueue.hasSpaceFor(6)) // http://en.wikipedia.org/wiki/Taxicab_geometry
                {
                    if (getLightValueAt(lightType, blockToUpdateX - 1, blockToUpdateY, blockToUpdateZ) < blockToUpdateNewLightValue)
                    {
                        updateLightQueue.push({blockToUpdateX - 1 - x, blockToUpdateY - y, blockToUpdateZ - z, 0});
                    }

                    if (getLightValueAt(lightType, blockToUpdateX + 1, blockToUpdateY, blockToUpdateZ) < blockToUpdateNewLightValue)
                    {
                        updateLightQueue.push({blockToUpdateX + 1 - x, blockToUpdateY - y, blockToUpdateZ - z, 0});
                    }

                    if (getLightValueAt(lightType, blockToUpdateX, blockToUpdateY - 1, blockToUpdateZ) < blockToUpdateNewLightValue)
                    {
                        updateLightQueue.push({blockToUpdateX - x, blockToUpdateY - 1 - y, blockToUpdateZ - z, 0});
                    }

                    if (getLightValueAt(lightType, blockToUpdateX, blockToUpdateY + 1, blockToUpdateZ) < blockToUpdateNewLightValue)
                    {
                        updateLightQueue.push({blockToUpdateX - x, blockToUpdateY + 1 - y, blockToUpdateZ - z, 0});
                    }

                    if (getLightValueAt(lightType, blockToUpdateX, blockToUpdateY, blockToUpdateZ - 1) < blockToUpdateNewLightValue)
                    {
                        updateLightQueue.push({blockToUpdateX - x, blockToUpdateY - y, blockToUpdateZ - 1 - z, 0});
                    }

                    if (getLightValueAt(lightType, blockToUpdateX, blockToUpdateY, blockToUpdateZ + 1) < blockToUpdateNewLightValue)
                    {
                        updateLightQueue.push({blockToUpdateX - x, blockToUpdateY - y, blockToUpdateZ + 1 - z, 0});
                    }
                }
            }
        }
    }
}

i_lightvalue World::computeBlockLightValueUsingNeighbors(eLightType lightType, int x, i_height y, int z)
{
    if (lightType == LIGHTTYPE_SKY && isBlockDirectlyLightedFromSky(x, y, z))
    {
        return LIGHT_VALUE_MAX;
    }

    i_block blockId = GetBlockId(x, y, z);
    i_lightvalue blockLightValue = 0;
    i_lightvalue blockLightOpacity = 0;
    const Block::Block* block = Block::BlockList::getBlock(blockId);
    if (block != nullptr)
    {
        blockLightValue = block->GetLightValue();
        blockLightOpacity = block->GetLightOpacity();
    }

    if (blockLightOpacity >= LIGHT_VALUE_MAX && blockLightValue > 0)
    {
        blockLightOpacity = 1;
    }

    if (blockLightOpacity < 1)
    {
        blockLightOpacity = 1;
    }

    if (lightType == LIGHTTYPE_SKY)
    {
        blockLightValue = 0;
    }


    if (blockLightOpacity >= LIGHT_VALUE_MAX)
    {
        return 0;
    }
    else if (blockLightValue >= LIGHT_VALUE_MAX - 1)
    {
        return blockLightValue;
    }
    else
    {
        FOR_EACH_SIDE_YZX(x, y, z, blockSide)
            int blockSideLightValue = getLightValueAt(lightType, blockSideX, blockSideY, blockSideZ) - blockLightOpacity;

            if (blockSideLightValue > blockLightValue)
            {
                blockLightValue = blockSideLightValue;
            }

            if (blockLightValue >= LIGHT_VALUE_MAX - 1)
            {
                return blockLightValue;
            }
        END_FOR_EACH_SIDE

        return blockLightValue;
    }

}

i_lightvalue World::getLightValueAt(eLightType lightType, int x, i_height y, int z)
{
    Chunk* chunk = GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk)
    {
        if (lightType == LIGHTTYPE_BLOCK)
            return chunk->getBlockLightAt(x & 0xf, y, z & 0xf);
        return chunk->getSkyLightAt(x & 0xf, y, z & 0xf);
    }
    return lightType;
}

void World::setLightValueAt(eLightType lightType, int x, i_height y, int z, i_lightvalue value)
{
    Chunk* chunk = GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk)
    {
        if (lightType == LIGHTTYPE_BLOCK)
            chunk->setBlockLightAt(x & 0xf, y, z & 0xf, value);
        else
            chunk->setSkyLightAt(x & 0xf, y, z & 0xf, value);
    }
}

void World::updateSkylightOnColumnt(int x, int z, i_height y1, i_height y2)
{
    if (!isChunksExistInRange(x, 0, z, 16))
        return;
    i_height ymin = std::min(y1, y2);
    i_height ymax = std::max(y1, y2);
    for (i_height y = ymin; y < ymax; y++)
    {
        updateLightByType(LIGHTTYPE_SKY, x, y, z);
    }
}

i_height World::getMinHeightMapAt(int x, int z)
{
    Chunk* chunk = GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk)
    {
        return chunk->getMinHeight();
    }
    return 0;
}

i_height World::getMinHeightAndHeightMapAt(int x, int z, i_height& heightMap)
{
    Chunk* chunk = GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk)
    {
        heightMap = chunk->getHeightMapAt(x & 0xf, z & 0xf);
        return chunk->getMinHeight();
    }
    heightMap = 0;
    return 0;
}

i_lightopacity World::GetBlockLightOpacity(int x, i_height y, int z)
{
    Chunk* chunk = GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk)
    {
        return chunk->getBlockLightOpacity(x & 0xf, y, z & 0xf);
    }
    return 0;
}

i_lightvalue World::GetRealLightValueAt(int x, i_height y, int z)
{
    return recursiveGetRealLightValueAt(x, y, z, true);
}

void World::MarkForNetworkUpdateTileEntity(int x, i_height y, int z)
{
    Chunk* chunk = GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk)
    {
        return chunk->MarkForNetworkUpdateTileEntity(x & 0xf, y, z & 0xf);
    }
}

Block::TileEntity* World::GetTileEntity(int x, i_height y, int z)
{
    Chunk* chunk = GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk)
    {
        return chunk->GetTileEntity(x & 0xf, y, z & 0xf);
    }
    return nullptr;
}

bool World::IsNormalCube(int x, i_height y, int z)
{
    i_block blockId = GetBlockId(x , y, z);
    const Block::Block* block = Block::BlockList::getBlock(blockId);
    return block && block->IsNormalCube();
}

void World::NotifyNeighborsForBlockChange(int x, i_height y, int z, i_block neighborBlockId)
{
    FOR_EACH_SIDE_XYZ(x, y, z, blockSide)
        NotifyNeighborBlockChange(blockSideX, blockSideY, blockSideZ, neighborBlockId);
    END_FOR_EACH_SIDE
}

void World::MarkPlayerForRemove(EntityPlayer* entity)
{
    playerToRemove.insert(entity);
}

i_lightvalue World::recursiveGetRealLightValueAt(int x, i_height y, int z, bool firstCall)
{
    if (firstCall)
    {
        i_block blockId = GetBlockId(x, y, z);
        const Block::Block* block = Block::BlockList::getBlock(blockId);
        if (block && block->UseNeighborBrightness())
        {
            i_lightvalue topLightValue = recursiveGetRealLightValueAt(x, std::max(255, y + 1), z, false);
            i_lightvalue eastLightValue = recursiveGetRealLightValueAt(x + 1, y, z, false);
            i_lightvalue westLightValue = recursiveGetRealLightValueAt(x - 1, y, z, false);
            i_lightvalue southLightValue = recursiveGetRealLightValueAt(x, y, z + 1, false);
            i_lightvalue northLightValue = recursiveGetRealLightValueAt(x, y, z - 1, false);

            if (eastLightValue > topLightValue)
            {
                topLightValue = eastLightValue;
            }

            if (westLightValue > topLightValue)
            {
                topLightValue = westLightValue;
            }

            if (southLightValue > topLightValue)
            {
                topLightValue = southLightValue;
            }

            if (northLightValue > topLightValue)
            {
                topLightValue = northLightValue;
            }

            return topLightValue;
        }
    }

    Chunk* chunk = GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk)
    {
        return chunk->getRealBlockLightValue(x & 0xf, y, z & 0xf, sunReduceValue);
    }
    return LIGHT_VALUE_MAX;
}

bool World::isBlockDirectlyLightedFromSky(int x, i_height y, int z)
{
    Chunk* chunk = GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk)
    {
        return y >= chunk->getHeightMapAt(x & 0xf, z & 0xf);
    }
    return false;
}

void World::SetTime(long long time)
{
    currentTime = time;
}




/*
    *
   public int getBlockPower(int x, int y, int z, int direction)


   public int computePowerLevelFromAroundBlock(int x, int y, int z)
   {

   }

   public boolean isBlockIndirectlyProvidingPowerTo(int x, int y, int z, int direction)
   {

   }

   public int getIndirectPowerLevel(int x, int y, int z, int direction)
   {

   }

   public boolean isBlockIndirectlyGettingPowered(int x, int y, int z) {

   }

   public int getMaxPowerFromBlockArround(int x, int y, int z)
   {

   }
 *
 */
i_powerlevel World::getBlockPower(int x, i_height y, int z, int direction)
{
    s_block_data blockData = GetBlockIdAndData(x, y, z);
    const Block::Block* block = Block::BlockList::getBlock(blockData.blockId);
    if (block)
    {
        return block->GetStrongPowerLevel(this, x, y, z, direction, blockData.blockData);
    }
    return 0;
}
i_powerlevel World::computePowerLevelFromAroundBlock(int x, i_height y, int z)
{
    i_powerlevel powerLevel = std::max(i_powerlevel(0), getBlockPower(x, y - 1, z, 0));

    if (powerLevel >= 15)
    {
        return powerLevel;
    }
    else
    {
        powerLevel = std::max(powerLevel, getBlockPower(x, y + 1, z, 1));

        if (powerLevel >= 15)
        {
            return powerLevel;
        }
        else
        {
            powerLevel = std::max(powerLevel, getBlockPower(x, y, z - 1, 2));

            if (powerLevel >= 15)
            {
                return powerLevel;
            }
            else
            {
                powerLevel = std::max(powerLevel, getBlockPower(x, y, z + 1, 3));

                if (powerLevel >= 15)
                {
                    return powerLevel;
                }
                else
                {
                    powerLevel = std::max(powerLevel, getBlockPower(x - 1, y, z, 4));

                    if (powerLevel >= 15)
                    {
                        return powerLevel;
                    }
                    else
                    {
                        powerLevel = std::max(powerLevel, getBlockPower(x + 1, y, z, 5));
                        return powerLevel;
                    }
                }
            }
        }
    }
}

bool World::isBlockIndirectlyProvidingPowerTo(int x, i_height y, int z, int direction)
{
    return getIndirectPowerLevel(x, y, z, direction) > 0;
}

i_powerlevel World::getIndirectPowerLevel(int x, i_height y, int z, int direction)
{
    if (IsNormalCube(x, y, z))
    {
        return computePowerLevelFromAroundBlock(x, y, z);
    }
    else
    {
        s_block_data blockData = GetBlockIdAndData(x, y, z);
        const Block::Block* block = Block::BlockList::getBlock(blockData.blockId);
        if (block)
        {
            return block->GetWeakPowerLevel(this, x, y, z, direction, blockData.blockData);
        }
        return 0;
    }
}

bool World::isBlockIndirectlyGettingPowered(int x, i_height y, int z)
{
    return getIndirectPowerLevel(x, y - 1, z, 0) > 0 ? true : (getIndirectPowerLevel(x, y + 1, z, 1) > 0 ? true : (getIndirectPowerLevel(x, y, z - 1, 2) > 0 ? true : (getIndirectPowerLevel(x, y, z + 1, 3) > 0 ? true : (getIndirectPowerLevel(x - 1, y, z, 4) > 0 ? true : getIndirectPowerLevel(x + 1, y, z, 5) > 0))));
}

i_powerlevel World::getMaxPowerFromBlockArround(int x, i_height y, int z)
{
    i_powerlevel maxValue = 0;

    FOR_EACH_SIDE_YZX(x, y, z, blockSide)
        int power = getIndirectPowerLevel(blockSideX, blockSideY, blockSideZ, side);

        if (power >= 15)
        {
            return 15;
        }

        if (power > maxValue)
        {
            maxValue = power;
        }
    }

    return maxValue;
}


Scripting::BlockRedstoneTorchBurnoutMgr* World::GetRedstoneTorchBurnoutMgr() const
{
    return redstoneTorchBurnoutMgr;
}


void World::load()
{
    std::stringstream fileName;
    fileName << worldName << "/level.dat";

    nbt::NbtFile nbtFile(fileName.str());
    try
    {
        nbtFile.read();
    }
    catch (nbt::GzipIOException& e)
    {
        LOG_ERROR << "No map found in world/ !" << std::endl;
        return;
    }

    nbt::Tag* root = nbtFile.getRoot();

    if (!root)
    {
        return;
    }

    nbt::TagCompound* rootCompound = dynamic_cast<nbt::TagCompound*>(root);
    if (!rootCompound)
    {
        return;
    }

    nbt::TagCompound* dataCompound = rootCompound->getValueAt<nbt::TagCompound>("Data");
    if (!dataCompound)
    {
        return;
    }

    loadSpawn(dataCompound);
    loadTimeAndWeather(dataCompound);
    loadGameMode(dataCompound);
}

void World::loadSpawn(nbt::TagCompound* tagData)
{
    nbt::TagInt* tagSpawnX = tagData->getValueAt<nbt::TagInt>("SpawnX");
    nbt::TagInt* tagSpawnY = tagData->getValueAt<nbt::TagInt>("SpawnY");
    nbt::TagInt* tagSpawnZ = tagData->getValueAt<nbt::TagInt>("SpawnZ");
    if (tagSpawnX && tagSpawnY && tagSpawnZ)
    {
        int spawnX = tagSpawnX->getValue();
        int spawnY = tagSpawnY->getValue();
        int spawnZ = tagSpawnZ->getValue();
        spawnPosition.Relocate(spawnX, spawnY, spawnZ);
    }
    else
    {
        spawnPosition.Relocate(0, 80, 0);
        LOG_ERROR << "Error while loading spawn data" << std::endl;
    }
}

void World::loadTimeAndWeather(nbt::TagCompound* tagData)
{
    nbt::TagLong* tagTime = tagData->getValueAt<nbt::TagLong>("Time");
    if (tagTime)
    {
        ageOfWorld = tagTime->getValue();
        currentTime = tagTime->getValue();
    }

    nbt::TagInt* tagRainTime = tagData->getValueAt<nbt::TagInt>("rainTime");
    if (tagRainTime)
    {
        rainTime = tagRainTime->getValue();
    }

    nbt::TagByte* tagRaining = tagData->getValueAt<nbt::TagByte>("raining");
    if (tagRaining)
    {
        raining = tagRaining->getValue() == 1;
    }

    nbt::TagInt* tagThunderTime = tagData->getValueAt<nbt::TagInt>("thunderTime");
    if (tagThunderTime)
    {
        thunderTime = tagThunderTime->getValue();
    }

    nbt::TagByte* tagThundering = tagData->getValueAt<nbt::TagByte>("thundering");
    if (tagThundering)
    {
        thundering = tagThundering->getValue() == 1;
    }

}

Position World::GetValidSpawnPosition()
{
    int spawnPosX = spawnPosition.x;
    int spawnPosY = spawnPosition.y;
    int spawnPosZ = spawnPosition.z;
    spawnPosX += (rand() % 16) - 8;
    spawnPosZ += (rand() % 16) - 8;

    Chunk* chunk = GetChunk(spawnPosX >> 4, spawnPosZ >> 4);
    i_block previousBlockId = chunk->getBlockAt(spawnPosX & 0xf, spawnPosY, spawnPosZ & 0xf);
    Position validPosition(spawnPosX, spawnPosY, spawnPosZ);
    for (int y = spawnPosY + 1; y < 260; y++)
    {
        i_block blockId = chunk->getBlockAt(spawnPosX & 0xf, y, spawnPosZ & 0xf);
        if (blockId == 0 && previousBlockId == 0)
        {
            break;
        }
        previousBlockId = blockId;
        validPosition.y++;
    }
    validPosition.y++;
    return validPosition;
}

nbt::NbtFile* World::LoadNbtDatasForPlayer(const std::string& playerName)
{
    std::stringstream fileName;
    fileName << worldName << "/players/" << playerName << ".dat";

    try
    {
        nbt::NbtFile* file = new nbt::NbtFile(fileName.str());
        file->read();
        return file;
    } catch(nbt::GzipIOException& e)
    {
        // No file found or corrupted file
        return nullptr;
    }
}

void World::loadGameMode(nbt::TagCompound* tagData)
{
    nbt::TagByte* tagHardcore = tagData->getValueAt<nbt::TagByte>("hardcore");
    if (tagHardcore)
    {
        hardcore = tagHardcore->getValue() == 1;
    }

    nbt::TagLong* tagRandoSeed = tagData->getValueAt<nbt::TagLong>("RandomSeed");
    if (tagRandoSeed)
    {
        seed = tagRandoSeed->getValue();
    }

    nbt::TagInt* tagGameType = tagData->getValueAt<nbt::TagInt>("GameType");
    if (tagGameType)
    {
        gameType = tagGameType->getValue();
    }
}

const Position& World::GetSpawnPosition() const
{
    return spawnPosition;
}

long long World::GetAgeOfWorld() const
{
    return ageOfWorld;
}

long long World::GetCurrentTime() const
{
    return currentTime;
}

bool World::isReadOnly() const
{
    return readOnly;
}

} /* namespace World */
