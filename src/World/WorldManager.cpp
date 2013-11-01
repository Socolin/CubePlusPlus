#include "WorldManager.h"

#include <iostream>
#include <cppnbt.h>
#include <fstream>
#include <libconfig.h++>

#include "Config/Config.h"
#include "Entity/EntityPlayer.h"
#include "Network/NetworkPacket.h"
#include "Network/NetworkSession.h"
#include "Network/OpcodeList.h"
#include "Util/StringUtil.h"
#include "Logging/Logger.h"
#include "World.h"

namespace World
{
WorldManager::WorldManager()
        : world(nullptr), isRunning(true), playerCount(0), maxPlayerCount(0), banFileName("ban"), adminFileName("admin"), motdArraySize(0), lateness(0)
{
    Config::Config::getConfig().lookupValue("server.general.maxplayers", maxPlayerCount);
    Config::Config::getConfig().lookupValue("server.general.name", serverName);
    Config::Config::getConfig().lookupValue("server.general.description", serverDescription);
    Config::Config::getConfig().lookupValue("server.general.online", onlineMode);
    Config::Config::getConfig().lookupValue("server.general.difficulty", difficulty);
    if (difficulty >= DIFFICULTY_MAX)
    {
        LOG_ERROR << "Invalid difficulty value: " << difficulty << ", max value is :" << DIFFICULTY_MAX - 1 << std::endl;
        difficulty = DIFFICULTY_MAX - 1;
    }
    Config::Config::getConfig().lookupValue("server.general.ban-file", banFileName);
    Config::Config::getConfig().lookupValue("server.general.admin-file", adminFileName);
    loadMotd();
    loadBanList();
    loadAdminList();
}

EntityPlayer* WorldManager::LoadAndJoinWorld(const std::wstring& name, Network::NetworkSession* session)
{
    if (IsBan(name))
    {
        session->kick(L"Banned !");
        return nullptr;
    }
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

    if (adminList.find(name) != adminList.end())
    {
        player->SetAdmin(true);
    }

    Network::NetworkPacket packet(Network::OP_LOGIN_REQUEST);
    std::wstring levelType(L"flat");
    packet  << int(-1) << levelType << (char)player->GetGameMode() << (char)0 << (char)difficulty << (char)0 << (maxPlayerCount > 120 ? char(120) : char(maxPlayerCount));
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

    Network::NetworkPacket packetChatMessage(Network::OP_CHAT_MESSAGE);
    for(int i=0; i<motdArraySize; i++)
    {
        std::wostringstream motdMessage;
        std::wstring motdWS (serverMotd[i].begin(), serverMotd[i].end());
        motdMessage << L"§a" << motdWS;
        player->SendChatMessage(motdMessage.str());
    }
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

const std::string& WorldManager::GetDescription() const
{
    return serverDescription;
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

void WorldManager::Kick(const std::wstring& playerName)
{
    auto playerItr = playerByNameList.find(playerName);
    if (playerItr != playerByNameList.end())
    {
        EntityPlayer* oldPlr = playerItr->second;
        if (oldPlr)
        {
            oldPlr->Kick(L"Kicked");
        }
    }
}

void WorldManager::Ban(const std::wstring& playerName)
{
    auto playerItr = playerByNameList.find(playerName);
    if (playerItr != playerByNameList.end())
    {
        EntityPlayer* oldPlr = playerItr->second;
        if (oldPlr)
        {
            oldPlr->Kick(L"Banned");
        }
    }
    if(!IsBan(playerName))
    {
        banList.insert(playerName);
        std::ofstream banFileList;
        std::string stringPlayerName;
        Util::WStringToString(playerName, stringPlayerName);
        banFileList.open(banFileName.c_str(), std::fstream::out | std::fstream::app);
        banFileList << stringPlayerName << std::endl;
        banFileList.close();
    }
}

void WorldManager::SetAdmin(const std::wstring& playerName)
{
    auto playerItr = playerByNameList.find(playerName);
    if (playerItr != playerByNameList.end())
    {
        EntityPlayer* oldPlr = playerItr->second;
        if (oldPlr)
        {
            oldPlr->SetAdmin(true);
        }
    }
    if(!IsAdmin(playerName))
    {
        adminList.insert(playerName);
        std::ofstream adminFileList;
        std::string stringPlayerName;
        Util::WStringToString(playerName, stringPlayerName);
        adminFileList.open(adminFileName.c_str(), std::fstream::out | std::fstream::app);
        adminFileList << stringPlayerName << std::endl;
        adminFileList.close();
    }
}

bool WorldManager::IsBan(const std::wstring& playerName)
{
    return (banList.find(playerName) != banList.end());
}

void WorldManager::Reload()
{
    loadBanList();
    loadAdminList();
}

void WorldManager::loadBanList()
{
    banList.clear();
    std::ifstream banFileList(banFileName.c_str());
    std::string line;
    while (std::getline(banFileList,line))
    {
        std::wstring playerName;
        Util::StringToWString(playerName, line);
        banList.insert(playerName);
    }
    banFileList.close();
}

void WorldManager::loadAdminList()
{
    adminList.clear();
    std::ifstream adminFileList(adminFileName.c_str());
    std::string line;
    while (std::getline(adminFileList,line))
    {
        std::wstring playerName;
        Util::StringToWString(playerName, line);
        adminList.insert(playerName);
    }
    adminFileList.close();
}

void WorldManager::loadMotd()
{
    libconfig::Setting& setting = Config::Config::getConfig().lookup("server.general");
    if(setting["motd"].isArray())
    {
        motdArraySize = setting["motd"].getLength();
        serverMotd = new std::string[motdArraySize];
        for(int i=0; i<motdArraySize; i++)
        {
            serverMotd[i] = setting["motd"][i].c_str();
        }
    }
}

bool WorldManager::IsAdmin(const std::wstring& playerName)
{
    return (adminList.find(playerName) != adminList.end());
}

int WorldManager::GetLateness() const
{
    return lateness;
}

void WorldManager::SetLateness(int lateness)
{
    this->lateness = lateness;
}

EntityPlayer* WorldManager::GetPlayerByName(const std::wstring& playerName)
{
    auto playerItr = playerByNameList.find(playerName);
    if (playerItr != playerByNameList.end())
    {
        EntityPlayer* oldPlr = playerItr->second;
        return oldPlr;
    }
    return nullptr;
}

std::map<std::wstring, EntityPlayer*>* WorldManager::GetPlayerByNameList()
{
    return &playerByNameList;
}

} /* namespace World */
