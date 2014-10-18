#include "World.h"

#include <cmath>
#include <sstream>
#include <algorithm>
#include <NBTField/NBTField.h>

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
#include "RegionManager.h"
#include "Util/FloatUtil.h"
#include "Util/types.h"
#include "VirtualChunk.h"
#include "VirtualSmallChunk.h"
#include "Config/Config.h"

namespace World
{

World::World(const std::string& worldName)
    : worldName(worldName)
    , worldPath(worldName + "/")
    , updateInProgress(false)
    , currentEntityId(10)
    , ageOfWorld(0)
    , currentTime(0)
    , rainTime(0)
    , raining(false)
    , thunderTime(false)
    , thundering(false)
    , weatherActivated(false)
    , hardcore(false)
    , seed(0)
    , gameType(0)
    , sunReduceValue(0)
{
    redstoneTorchBurnoutMgr = new Scripting::BlockRedstoneTorchBurnoutMgr();
    Config::Config::GetConfig().lookupValue("server.world.read-only", readOnly);
    Config::Config::GetConfig().lookupValue("server.world.time.locked", lockedTime);
    Config::Config::GetConfig().lookupValue("server.world.time.locked-value", lockedTimeValue);
    Config::Config::GetConfig().lookupValue("server.world.time.locked-value", lockedTimeValue);
    Config::Config::GetConfig().lookupValue("server.world.chunk.update-tick", enableUpdateChunk);
    Config::Config::GetConfig().lookupValue("server.world.view-distance", viewDistance);
    if (viewDistance < 7 || viewDistance > 15)
    {
        LOG_ERROR << "Invalid viewDistance value: " << viewDistance << ", value must be between 7 and 15" << std::endl;
        viewDistance = 10;
    }
    Config::Config::GetConfig().lookupValue("server.world.spawn-distance", spawnDistance);
    if (spawnDistance <= 0)
    {
        LOG_ERROR << "Invalid spawnDistance value: " << spawnDistance << ", value must be greater than 0" << std::endl;
        spawnDistance = 16;
    }
    Config::Config::GetConfig().lookupValue("server.general.world-dir", worldPath);
    regionManager = RegionManager(worldPath + "/");
    load();
}

World::~World()
{
    for (std::pair<long long, Chunk*> chunk : chunkMap)
    {
        chunk.second->Unload();
        delete chunk.second;
    }
    for (std::pair<long long, VirtualChunk*> chunk : virtualChunkMap)
    {
        chunk.second->Unload();
        delete chunk.second;
    }
    for (std::pair<long long, VirtualSmallChunk*> chunk : virtualSmallChunkMap)
    {
        delete chunk.second;
    }

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

/*************************************************************************
 * General world managmenet
 *************************************************************************/

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

    updateTime();
    updateEntities();
}


void World::Save() const
{
    if (IsReadOnly())
        return;

    NBT::TagCompound* tagData = new NBT::TagCompound();

    saveSpawn(tagData);
    saveTimeAndWeather(tagData);
    saveGameMode(tagData);

    std::stringstream fileName;
    fileName << worldPath << "/level.dat";

    NBT::File nbtFile = NBT::File(fileName.str(), tagData);
    if (!nbtFile.Save())
    {
        LOG_ERROR << "Error while saving world data: " << nbtFile.GetFilename() << " : "
                << nbtFile.GetLastErrorMessage() << std::endl;
        return;
    }

    for (EntityPlayer* player : playerList)
    {
        NBT::TagCompound* playerData = new NBT::TagCompound();
        if (player->Save(playerData))
        {
            std::string name;
            Util::WStringToString(player->GetUsername(), name);
            SaveNbtDatasForPlayer(name, playerData);
        }
        else
            delete playerData;
    }

    LOG_DEBUG << "Saving: " << chunkMap.size() << " active chunks" << std::endl;
    size_t saveCount = 0;
    for (auto chunkItr : chunkMap)
    {
        Chunk* chunk = chunkItr.second;
        if (chunk->NeedSave())
        {
            saveCount++;
            chunk->Save();
        }
    }
    LOG_DEBUG << saveCount << " chunks saved" << std::endl;
}

void World::load()
{
    std::stringstream fileName;
    fileName << worldPath << "/level.dat";

    NBT::File nbtFile = NBT::File(fileName.str());
    if (!nbtFile.Load())
    {
        LOG_ERROR << "Error while loading world data: " << nbtFile.GetFilename() << " : "
                << nbtFile.GetLastErrorMessage() << std::endl;
        return;
    }

    NBT::Tag* root = nbtFile.GetRoot();
    if (!root)
    {
        return;
    }

    NBT::TagCompound* rootCompound = root->GetTagAs<NBT::TagCompound>();
    if (!rootCompound)
    {
        return;
    }

    NBT::TagCompound* dataCompound = rootCompound->GetTagAs<NBT::TagCompound>("Data");
    if (!dataCompound)
    {
        return;
    }

    loadSpawn(dataCompound);
    loadTimeAndWeather(dataCompound);
    loadGameMode(dataCompound);
}




/*************************************************************************
 * Entities managmenet
 *************************************************************************/

void World::AddEntity(Entity* entity)
{
    entity->SetWorld(this, currentEntityId++);

    // Add entity to virtual chunk, so all player in sight range will be notified that new entity spawn
    VirtualChunk* virtualChunk = GetVirtualChunk(((int) entity->x) >> 7, ((int) entity->z) >> 7);
    virtualChunk->AddEntity(entity);

    // Add entity to chunk to be find
    VirtualSmallChunk *vChunk = GetVirtualSmallChunk(((int) entity->x) >> 4, ((int) entity->z) >> 4);
    vChunk->AddEntity(entity);

    // Add entity to be find by id search
    entityById[entity->GetEntityId()] = entity;

    // Notify entity that it join world
    entity->OnJoinWorld(this);
}

void World::AddPlayer(EntityPlayer* player)
{
    player->SetWorld(this, currentEntityId++);
    playerList.insert(player);
    int chunkX = ((int) player->x) >> 4;
    int chunkZ = ((int) player->z) >> 4;

    // Add player to a virtual small chunk, that are used to find entity in world, in specific area.
    VirtualSmallChunk *vChunk = GetVirtualSmallChunk(chunkX, chunkZ);
    vChunk->AddPlayer(player);

    // Add player in chunk, so he will be notified of all change that would happen
    // on them.
    int maxChunkX = chunkX + viewDistance;
    int maxChunkZ = chunkZ + viewDistance;

    for (int x = chunkX - viewDistance; x <= maxChunkX; x++)
        for (int z = chunkZ - viewDistance; z <= maxChunkZ; z++)
        {
            Chunk* chunk = GetChunk(x, z);
            chunk->AddPlayer(player);
            player->AddChunkToSend(x, z);
        }

    // Add player to virtual chunk, then he will receive other entity data,
    // and other player will receive that the player appear
    VirtualChunk* virtualChunk = GetVirtualChunk(((int) player->x) >> 7, ((int) player->z) >> 7);
    virtualChunk->AddPlayer(player);

    // Register player in world to be find by id
    entityById[player->GetEntityId()] = player;

    // Notify player that he join world
    player->OnJoinWorld(this);
}

void World::MarkPlayerForRemove(EntityPlayer* entity)
{
    playerToRemove.insert(entity);
}

void World::MarkEntityAsDead(int entityId)
{
    deadEntity.insert(entityId);
}

Entity* World::GetEntityById(int entityId)
{
    auto entityItr = entityById.find(entityId);
    if (entityItr == entityById.end())
        return nullptr;
    return entityItr->second;
}

NBT::TagCompound* World::LoadNbtDatasForPlayer(const std::string& playerName)
{
    std::stringstream fileName;
    fileName << worldPath << "/players/" << playerName << ".dat";

    NBT::File file(fileName.str());
    if (!file.Load())
    {
        // No file found or corrupted file
        // TODO: maybe add check on error, to know if it's first
        // connection of player, or an error while loading nbt
        return nullptr;
    }

    NBT::Tag* root = file.TakeRoot();
    NBT::TagCompound* rootAsCompound = nullptr;
    if (root)
    {
        rootAsCompound = root->GetTagAs<NBT::TagCompound>();
        if (rootAsCompound == nullptr)
        {
            delete root;
        }
    }
    return rootAsCompound;
}


void World::SaveNbtDatasForPlayer(const std::string& playerName, NBT::TagCompound* tagData) const
{
    std::stringstream fileName;
    fileName << worldPath << "/players/" << playerName << ".dat";

    NBT::File file(fileName.str(), tagData);
    if (!file.Save())
    {
        LOG_ERROR << "Can't save player data: " << playerName
                << " due to error:" << file.GetLastErrorMessage()
                << " when writing: "  << file.GetFilename() << std::endl;
    }
}

void World::updateEntities()
{
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
                    removePlayer(player);
                    continue;
                }
            }
            removeEntity(entity, true);
        }
    }
    deadEntity.clear();
    for (size_t i = 0; i < entityToDelete.size(); i++)
    {
        delete entityToDelete[i];
    }
    entityToDelete.clear();
    for (EntityPlayer* player : playerToRemove) {
        removePlayer(player);
        delete player;
    }
    playerToRemove.clear();
}

VirtualChunk* World::createVirtualChunk(int x, int z)
{
    VirtualChunk* vChunk = new VirtualChunk(x, z, this);
    return vChunk;
}

VirtualSmallChunk* World::createVirtualSmallChunk(int x, int z)
{
    VirtualSmallChunk* vChunk = new VirtualSmallChunk(x, z, this);
    return vChunk;
}

void World::removeEntity(Entity* entity, bool deleteEntity)
{
    // Remove entity and notify player that can see it
    VirtualChunk* virtualChunk = GetVirtualChunk(((int) entity->x) >> 7, ((int) entity->z) >> 7);
    virtualChunk->RemoveEntity(entity);

    VirtualSmallChunk *vChunk = GetVirtualSmallChunk(((int) entity->x) >> 4, ((int) entity->z) >> 4);
    vChunk->RemoveEntity(entity);

    entityById[entity->GetEntityId()] = nullptr;

    entity->SetWorld(nullptr, 0);

    if (deleteEntity)
        delete entity;
}

void World::removePlayer(EntityPlayer* player)
{
    if (!IsReadOnly())
    {
        NBT::TagCompound* playerData = new NBT::TagCompound();
        if (player->Save(playerData))
        {
            std::string name;
            Util::WStringToString(player->GetUsername(), name);
            SaveNbtDatasForPlayer(name, playerData);
        }
        else
        {
            delete playerData;
        }
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
    entityById[player->GetEntityId()] = nullptr;
    player->SetWorld(nullptr, 0);
}

void World::markEntityForDelete(Entity* entity)
{
    entityToDelete.push_back(entity);
}




/*************************************************************************
 * Chunks managmenet
 *************************************************************************/

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

Chunk* World::loadChunk(int x, int z)
{
    Chunk* chunk = new Chunk(x, z, this);
    chunk->Load();
    chunkMap[CHUNK_KEY(x,z)] = chunk;
    return chunk;
}

NBT::TagCompound* World::getChunkNbtData(int x, int z)
{
    Region* region = regionManager.GetRegion(x >> 5, z >> 5);
    if (region)
    {
        return region->GetNbtChunkData(x & 0x1f, z & 0x1f);
    }
    return nullptr;
}

void World::saveChunkNbtData(int x, int z, NBT::TagCompound* tag)
{
    Region* region = regionManager.GetRegion(x >> 5, z >> 5);
    if (region)
    {
        return region->SaveNbtChunkData(x & 0x1f, z & 0x1f, tag);
    }
}

bool World::isChunksExistInRange(int x, int z, int range) const
{
    return isChunksExist(x - range, z - range, x + range, z + range);
}

bool World::isChunksExist(int xmin, int zmin, int xmax, int zmax) const
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

bool World::isChunkExist(int chunkX, int chunkZ) const
{
    Chunk* chunk = GetChunkIfLoaded(chunkX, chunkZ);
    return chunk != nullptr;
}

i_height World::getMinHeightMapAt(int x, int z) const
{
    Chunk* chunk = GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk)
    {
        return chunk->getMinHeight();
    }
    return 0;
}

i_height World::getMinHeightAndHeightMapAt(int x, int z, i_height& heightMap) const
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




/*************************************************************************
 * Weather and time managmenet
 *************************************************************************/

void World::SetTime(long long time)
{
    currentTime = time;
}

long long World::GetAgeOfWorld() const
{
    return ageOfWorld;
}

long long World::GetCurrentTime() const
{
    return currentTime;

    //TODO : In 1.6, if negative the time will stop moving on client.
}

void World::updateTime()
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

        //TODO : In 1.6, if negative the time will stop moving on client
    }

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

void World::loadTimeAndWeather(NBT::TagCompound* tagData)
{
    NBT::TagLong* tagTime = tagData->GetTagAs<NBT::TagLong>("Time");
    if (tagTime)
    {
        ageOfWorld = tagTime->GetValue();
        currentTime = tagTime->GetValue();
    }

    NBT::TagInt* tagRainTime = tagData->GetTagAs<NBT::TagInt>("rainTime");
    if (tagRainTime)
    {
        rainTime = tagRainTime->GetValue();
    }

    NBT::TagByte* tagRaining = tagData->GetTagAs<NBT::TagByte>("raining");
    if (tagRaining)
    {
        raining = tagRaining->GetValue() == 1;
    }

    NBT::TagInt* tagThunderTime = tagData->GetTagAs<NBT::TagInt>("thunderTime");
    if (tagThunderTime)
    {
        thunderTime = tagThunderTime->GetValue();
    }

    NBT::TagByte* tagThundering = tagData->GetTagAs<NBT::TagByte>("thundering");
    if (tagThundering)
    {
        thundering = tagThundering->GetValue() == 1;
    }
}

void World::saveTimeAndWeather(NBT::TagCompound* tagData) const
{
    tagData->AddLong("Time", ageOfWorld);
    tagData->AddInt("rainTime", rainTime);
    tagData->AddByte("raining", raining ? 1 : 0);
    tagData->AddInt("thunderTime", thunderTime);
    tagData->AddByte("thundering", thundering ? 1 : 0);
}




/*************************************************************************
 * World parameters
 *************************************************************************/

int World::GetViewDistance() const
{
    return viewDistance;
}

int World::GetGameType() const
{
    return gameType;
}

bool World::IsReadOnly() const
{
    return readOnly;
}

const Position& World::GetSpawnPosition() const
{
    return spawnPosition;
}

Position World::GetValidSpawnPosition()
{
    int spawnPosX = spawnPosition.x;
    int spawnPosY = spawnPosition.y;
    int spawnPosZ = spawnPosition.z;
    spawnPosX += (rand() % spawnDistance) - (spawnDistance / 2);
    spawnPosZ += (rand() % spawnDistance) - (spawnDistance / 2);

    Chunk* chunk = GetChunk(spawnPosX >> 4, spawnPosZ >> 4);
    i_block previousBlockId = chunk->getBlockAt(spawnPosX & 0xf, spawnPosY, spawnPosZ & 0xf);
    Position validPosition(spawnPosX, spawnPosY, spawnPosZ);
    for (int y = spawnPosY + 1; y < 260; y++)
    {
        i_block blockId = chunk->getBlockAt(spawnPosX & 0xf, y, spawnPosZ & 0xf);
        if (blockId == 0 && previousBlockId == 0)
        {
            validPosition.y = y - 1;
            break;
        }
        previousBlockId = blockId;
    }
    return validPosition;
}

void World::loadSpawn(NBT::TagCompound* tagData)
{
    NBT::TagInt* tagSpawnX = tagData->GetTagAs<NBT::TagInt>("SpawnX");
    NBT::TagInt* tagSpawnY = tagData->GetTagAs<NBT::TagInt>("SpawnY");
    NBT::TagInt* tagSpawnZ = tagData->GetTagAs<NBT::TagInt>("SpawnZ");
    if (tagSpawnX && tagSpawnY && tagSpawnZ)
    {
        int spawnX = tagSpawnX->GetValue();
        int spawnY = tagSpawnY->GetValue();
        int spawnZ = tagSpawnZ->GetValue();
        spawnPosition.Relocate(spawnX, spawnY, spawnZ);
    }
    else
    {
        spawnPosition.Relocate(0, 80, 0);
        LOG_ERROR << "Error while loading spawn data" << std::endl;
    }
}

void World::loadGameMode(NBT::TagCompound* tagData)
{
    NBT::TagByte* tagHardcore = tagData->GetTagAs<NBT::TagByte>("hardcore");
    if (tagHardcore)
    {
        hardcore = tagHardcore->GetValue() == 1;
    }

    NBT::TagLong* tagRandoSeed = tagData->GetTagAs<NBT::TagLong>("RandomSeed");
    if (tagRandoSeed)
    {
        seed = tagRandoSeed->GetValue();
    }

    NBT::TagInt* tagGameType = tagData->GetTagAs<NBT::TagInt>("GameType");
    if (tagGameType)
    {
        gameType = tagGameType->GetValue();
    }
}

void World::saveSpawn(NBT::TagCompound* tagData) const
{
    tagData->AddInt("SpawnX", std::floor(spawnPosition.x));
    tagData->AddInt("SpawnY", std::floor(spawnPosition.y));
    tagData->AddInt("SpawnZ", std::floor(spawnPosition.z));
}

void World::saveGameMode(NBT::TagCompound* tagData) const
{
    tagData->AddByte("hardcore", hardcore ? 1 : 0);
    tagData->AddLong("RandomSeed", seed);
    tagData->AddInt("GameType", gameType);
}




/*************************************************************************
 * Block management
 *************************************************************************/

void World::ChangeBlock(int x, i_height y, int z, i_block blockId, i_data blockData, bool playSound)
{
    Chunk* chunk = GetChunk(x >> 4, z >> 4);
    chunk->ChangeBlock(x & 0xf, y, z & 0xf, blockId, blockData);

    if (playSound)
    {
        const Block::Block* block = Block::BlockList::getBlock(blockId);
        if (block)
        {
            //TODO: change, use world method to play sound
            const Block::SoundBlock& sound = block->GetSound();
            Network::NetworkPacket soundPacket(Network::OP_NAMED_SOUND_EFFECT);
            soundPacket << sound.GetPlaceSound() << (x * 8) << (y * 8) << (z * 8)
                    << sound.GetVolume() << (char)(sound.GetModifier() * 63.f);
            VirtualSmallChunk* vSmallChunk = GetVirtualSmallChunk(x >> 4, z >> 4);
            vSmallChunk->SendPacketToAllNearPlayer(soundPacket);
        }
    }

    NotifyNeighborsForBlockChange(x, y, z, blockId);

    updateAllLightTypes(x, y, z);
}

void World::ChangeBlockNoEvent(int x, i_height y, int z, i_block blockId, i_data blockData)
{
    Chunk* chunk = GetChunk(x >> 4, z >> 4);
    chunk->ChangeBlock(x & 0xf, y, z & 0xf, blockId, blockData);
    updateAllLightTypes(x, y, z);
}

void World::ChangeDataNotify(int x, i_height y, int z, i_data blockData)
{
    Chunk* chunk = GetChunk(x >> 4, z >> 4);
    chunk->ChangeData(x & 0xf, y, z & 0xf, blockData);

    i_block blockId = chunk->getBlockAt(x & 0xf, y, z & 0xf);
    NotifyNeighborsForBlockChange(x, y, z, blockId);
}

void World::ChangeDataNoEvent(int x, i_height y, int z, i_data blockData)
{
    Chunk* chunk = GetChunk(x >> 4, z >> 4);
    chunk->ChangeData(x & 0xf, y, z & 0xf, blockData);
}

void World::BreakBlock(int x, i_height y, int z)
{
    Chunk* chunk = GetChunk(x >> 4, z >> 4);
    i_block blockId = chunk->getBlockAt(x & 0xf, y, z & 0xf);
    if (blockId > 0)
    {
        const Block::Block* block = Block::BlockList::getBlock(blockId);
        if (block != nullptr)
        {
            block->Drop(this, x, y, z);
            chunk->ChangeBlock(x & 0xf, y, z & 0xf, block->GetReplaceBlockId(), 0);
        }
        else
        {
            chunk->ChangeBlock(x & 0xf, y, z & 0xf, 0, 0);
        }

        NotifyNeighborsForBlockChange(x, y, z, 0);
    }
    updateAllLightTypes(x, y, z);
}

void World::RemoveBlock(int x, i_height y, int z)
{
    Chunk* chunk = GetChunk(x >> 4, z >> 4);
    i_block blockId = chunk->getBlockAt(x & 0xf, y, z & 0xf);
    if (blockId > 0)
    {
        const Block::Block* block = Block::BlockList::getBlock(blockId);
        if (block != nullptr)
        {
            chunk->ChangeBlock(x & 0xf, y, z & 0xf, block->GetReplaceBlockId(), 0);
        }
        else
        {
            chunk->ChangeBlock(x & 0xf, y, z & 0xf, 0, 0);
        }

        NotifyNeighborsForBlockChange(x, y, z, 0);
    }
    updateAllLightTypes(x, y, z);
}

void World::MarkBlockForUpdate(int x, i_height y, int z, i_block blockId, unsigned int waitTick)
{
    Chunk* chunk = GetChunk(x >> 4, z >> 4);
    chunk->MarkForUpdate(x & 0xf, y, z & 0xf, blockId, waitTick);
}

void World::NotifyNeighborsForBlockChange(int x, i_height y, int z, i_block neighborBlockId)
{
    FOR_EACH_SIDE_XYZ(x, y, z, blockSide)
        NotifyNeighborBlockChange(blockSideX, blockSideY, blockSideZ, neighborBlockId);
    END_FOR_EACH_SIDE
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

Block::TileEntity* World::GetTileEntity(int x, i_height y, int z)
{
    Chunk* chunk = GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk)
    {
        return chunk->GetTileEntity(x & 0xf, y, z & 0xf);
    }
    return nullptr;
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

void World::MarkForNetworkUpdateTileEntity(int x, i_height y, int z)
{
    Chunk* chunk = GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk)
    {
        return chunk->MarkForNetworkUpdateTileEntity(x & 0xf, y, z & 0xf);
    }
}




/*************************************************************************
 * Blocks helper
 *************************************************************************/

bool World::IsNormalCube(int x, i_height y, int z)
{
    i_block blockId = GetBlockId(x , y, z);
    const Block::Block* block = Block::BlockList::getBlock(blockId);
    return block && block->IsNormalCube();
}




/*************************************************************************
 * Sound management
 *************************************************************************/

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




/*************************************************************************
 * Network management
 *************************************************************************/

void World::UpdateGameState(char reason, char gameMode)
{
    Network::NetworkPacket packet(Network::OP_CHANGE_GAME_STATE);
    packet << reason << gameMode;
    SendPacketToPlayerInWorld(packet);
}

void World::SendPacketToPlayerInWorld(const Network::NetworkPacket& packet) const
{
    for (EntityPlayer* plr : playerList)
    {
        plr->Send(packet);
    }
}




/*************************************************************************
 * Item management
 *************************************************************************/

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




/*************************************************************************
 * Collision management
 *************************************************************************/

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




/*************************************************************************
 * Light management
 *************************************************************************/

void World::updateAllLightTypes(int x, i_height y, int z)
{
    updateLightByType(LIGHTTYPE_SKY, x, y, z);
    updateLightByType(LIGHTTYPE_BLOCK, x, y, z);
}

void World::updateLightByType(eLightType lightType, int x, i_height y, int z)
{
    if (!isChunksExistInRange(x, z, 17))
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

            i_lightvalue blockToUpdateLightValue = blockToUpdate.level;
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
    if (!isChunksExistInRange(x, z, 16))
        return;
    i_height ymin = std::min(y1, y2);
    i_height ymax = std::max(y1, y2);
    for (i_height y = ymin; y < ymax; y++)
    {
        updateLightByType(LIGHTTYPE_SKY, x, y, z);
    }
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




/*************************************************************************
 * Redstone management
 *************************************************************************/

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
    END_FOR_EACH_SIDE

    return maxValue;
}


Scripting::BlockRedstoneTorchBurnoutMgr* World::GetRedstoneTorchBurnoutMgr() const
{
    return redstoneTorchBurnoutMgr;
}

} /* namespace World */
