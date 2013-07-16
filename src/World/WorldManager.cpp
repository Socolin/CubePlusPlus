#include "WorldManager.h"

#include <iostream>
#include <cppnbt.h>

#include "Config/Config.h"
#include "Entity/EntityPlayer.h"
#include "Network/NetworkPacket.h"
#include "Network/NetworkSession.h"
#include "Network/OpcodeList.h"
#include "Util/StringUtil.h"
#include "World.h"

namespace World
{
WorldManager::WorldManager()
        : world(nullptr), isRunning(true), playerCount(0), maxPlayerCount(0)
{
    Config::Config::getConfig().lookupValue("server.general.maxplayers", maxPlayerCount);
    Config::Config::getConfig().lookupValue("server.general.name", serverName);
    Config::Config::getConfig().lookupValue("server.general.motd", serverMotd);
    Config::Config::getConfig().lookupValue("server.general.online", onlineMode);
    Config::Config::getConfig().lookupValue("server.general.difficulty", difficulty);
    if (difficulty >= DIFFICULTY_MAX)
    {
        std::cerr << "Invalid difficulty value: " << difficulty << ", max value is :" << DIFFICULTY_MAX - 1 << std::endl;
        difficulty = DIFFICULTY_MAX - 1;
    }
}

EntityPlayer* WorldManager::LoadAndJoinWorld(const std::wstring& name, Network::NetworkSession* session)
{
    auto playerItr = playerByNameList.find(name);
    if (playerItr != playerByNameList.end())
    {
        EntityPlayer* oldPlr = playerItr->second;
        if (oldPlr)
        {
            oldPlr->Kick(L"Logged from an other location");
            RemovePlayer(oldPlr);
        }
    }
    EntityPlayer* player = new EntityPlayer(world->GetValidSpawnPosition(), name, session);

    Network::NetworkPacket packet(Network::OP_LOGIN_REQUEST);
    std::wstring levelType(L"flat");
    packet  << int(-1) << levelType << (char)1 << (char)0 << (char)difficulty << (char)0 << (maxPlayerCount > 120 ? char(120) : char(maxPlayerCount));
    session->SendPacket(packet);

    std::string stringName;
    Util::WStringToString(name, stringName);
    nbt::NbtFile* file = world->LoadNbtDatasForPlayer(stringName);
    if (file != nullptr)
    {
        nbt::Tag* tag = file->getRoot();
        if (tag)
        {
            nbt::TagCompound* tagCompound = dynamic_cast<nbt::TagCompound*>(tag);
            if (tagCompound)
            {
                player->Load(tagCompound);
            }
        }
    }
    world->AddPlayer(player);

    if (playerList.find(player) == playerList.end())
        playerCount++;
    playerList.insert(player);

    playerByNameList[name] = player;

    return player;
}

void WorldManager::RemovePlayer(EntityPlayer* player)
{
    if (playerList.find(player) != playerList.end())
    {
        playerCount--;
        world->RemovePlayer(player);
        playerList.erase(player);
        playerByNameList.erase(player->GetUsername());
        // TODO: fiind where delete player
    }
}

bool WorldManager::IsRunning()
{
    return isRunning;
}

void WorldManager::HandleChatMessage(EntityPlayer* player, std::wstring& message)
{
    bool isCanceled = chatManager.HandleChatMessage(player, message);
    if (!isCanceled)
    {
        Network::NetworkPacket packetChatMessage(Network::OP_CHAT_MESSAGE);
        packetChatMessage << message;
        SendToAllPlayer(packetChatMessage);
    }
}

World* WorldManager::GetWorld() const
{
    return world;
}

WorldManager::~WorldManager()
{
    world->Unload();
    delete world;
    world = nullptr;
}

void WorldManager::UpdateTick() const
{
    world->UpdateTick();
}

void WorldManager::Init()
{
    world = new World("world");
}
void WorldManager::Stop()
{
    std::wstring kickReason(L"Server shutting down");
    isRunning = false;
    for (auto itrPlr = playerList.begin(); itrPlr != playerList.end();)
    {
        EntityPlayer* toKick = *itrPlr;
        itrPlr++;
        toKick->Kick(kickReason);
    }
}
void WorldManager::SendToAllPlayer(const Network::NetworkPacket& packet) const
{
    for (EntityPlayer* plr : playerList)
    {
        plr->Send(packet);
    }
}

const std::string& WorldManager::GetName() const
{
    return serverName;
}
const std::string& WorldManager::GetMotd() const
{
    return serverMotd;
}

int WorldManager::GetPlayerCount() const
{
    return playerCount;
}

int WorldManager::GetMaxPlayerCount() const
{
    return maxPlayerCount;
}

void WorldManager::SetMaxPlayerCount(int maxPlayerCount)
{
    this->maxPlayerCount = maxPlayerCount;
}

bool WorldManager::IsFull() const
{
    return maxPlayerCount <= playerCount;
}

bool WorldManager::IsOnlineMode() const
{
    return onlineMode;
}

} /* namespace World */
