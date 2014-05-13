#include "WorldManager.h"

#include <iostream>
#include <fstream>
#include <libconfig.h++>
#include <NBTField/NBTField.h>

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
    : world(nullptr)
    , isRunning(true)
    , playerCount(0)
    , maxPlayerCount(0)
    , banFileName("ban")
    , adminFileName("admin")
    , whitelistFileName("whitelist")
    , mutedPlayersFileName("mutedPlayers")
    , useWhitelist(false)
    , motdArraySize(0)
    , lateness(0)
{
    Config::Config::GetConfig().lookupValue("server.general.maxplayers", maxPlayerCount);
    Config::Config::GetConfig().lookupValue("server.general.name", serverName);
    Config::Config::GetConfig().lookupValue("server.general.description", serverDescription);
    Config::Config::GetConfig().lookupValue("server.general.online", onlineMode);
    Config::Config::GetConfig().lookupValue("server.general.difficulty", difficulty);
    if (difficulty >= DIFFICULTY_MAX)
    {
        LOG_ERROR << "Invalid difficulty value: " << difficulty << ", max value is :" << DIFFICULTY_MAX - 1 << std::endl;
        difficulty = DIFFICULTY_MAX - 1;
    }
    Config::Config::GetConfig().lookupValue("server.general.ban-file", banFileName);
    Config::Config::GetConfig().lookupValue("server.general.admin-file", adminFileName);
    Config::Config::GetConfig().lookupValue("server.general.whitelist", useWhitelist);
    Config::Config::GetConfig().lookupValue("server.general.muted-players-file", mutedPlayersFileName);
    if(useWhitelist)
    {
        Config::Config::GetConfig().lookupValue("server.general.whitelist-file", whitelistFileName);
        loadWhitelist();
    }
    loadMotd();
    loadBanList();
    loadAdminList();
    loadMutedPlayersList();
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

    if (IsAdmin(name))
    {
        player->SetAdmin(true);
    }
    if (IsMuted(name))
    {
        player->GetChat().Mute(true);
    }

    Network::NetworkPacket packet(Network::OP_LOGIN_REQUEST);
    std::wstring levelType(L"flat");
    packet  << int(-1) << levelType << (char)player->GetGameMode() << (char)0 << (char)difficulty << (char)0 << (maxPlayerCount > 120 ? char(120) : char(maxPlayerCount));
    session->SendPacket(packet);

    std::string stringName;
    Util::WStringToString(name, stringName);

    NBT::TagCompound* playerNbtData = world->LoadNbtDatasForPlayer(stringName);
    if (playerNbtData)
    {
        player->Load(playerNbtData);
        delete playerNbtData;
    }

    world->AddPlayer(player);

    if (playerList.find(player) == playerList.end())
        playerCount++;
    playerList.insert(player);

    playerByNameList[name] = player;

    Network::NetworkPacket packetChatMessage(Network::OP_CHAT_MESSAGE);
    for(int i = 0; i < motdArraySize; i++)
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
        world->MarkPlayerForRemove(player);
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
    delete world;
    world = nullptr;
}

void WorldManager::UpdateTick() const
{
    world->UpdateTick();
}

void WorldManager::Init()
{
    //TODO: remove name, load from config
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
    world->Save();
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

bool WorldManager::Ban(const std::wstring& playerName)
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
        return true;
    }
    return false;
}

bool WorldManager::UnBan(const std::wstring& playerName)
{
    if(IsBan(playerName))
    {
        banList.erase(playerName);
        std::ofstream banFileList;
        std::string stringPlayerName;
        banFileList.open(banFileName.c_str(), std::fstream::out | std::fstream::trunc);
        for (auto itrPlr = banList.begin(); itrPlr != banList.end(); itrPlr++)
        {
            Util::WStringToString(*itrPlr, stringPlayerName);
            banFileList << stringPlayerName << std::endl;
        }
        banFileList.close();
        return true;
    }
    return false;
}

bool WorldManager::SetAdmin(const std::wstring& playerName)
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
        return true;
    }
    return false;
}

bool WorldManager::UnAdmin(const std::wstring& playerName)
{
    auto playerItr = playerByNameList.find(playerName);
    if (playerItr != playerByNameList.end())
    {
        EntityPlayer* oldPlr = playerItr->second;
        if (oldPlr)
        {
            oldPlr->SetAdmin(false);
        }
    }
    if(IsAdmin(playerName))
    {
        adminList.erase(playerName);
        std::ofstream adminFileList;
        std::string stringPlayerName;
        adminFileList.open(adminFileName.c_str(), std::fstream::out | std::fstream::trunc);
        for (auto itrPlr = adminList.begin(); itrPlr != adminList.end(); itrPlr++)
        {
            Util::WStringToString(*itrPlr, stringPlayerName);
            adminFileList << stringPlayerName << std::endl;
        }
        adminFileList.close();
        return true;
    }
    return false;
}

bool WorldManager::AddToWhitelist(const std::wstring& playerName)
{
    if(!IsWhitelisted(playerName))
    {
        whitelist.insert(playerName);
        std::ofstream whitelistFile;
        std::string stringPlayerName;
        Util::WStringToString(playerName, stringPlayerName);
        whitelistFile.open(whitelistFileName.c_str(), std::fstream::out | std::fstream::app);
        whitelistFile << stringPlayerName << std::endl;
        whitelistFile.close();
        return true;
    }
    return false;
}

bool WorldManager::UnWhitelist(const std::wstring& playerName)
{
    auto playerItr = playerByNameList.find(playerName);
    if (playerItr != playerByNameList.end())
    {
        EntityPlayer* oldPlr = playerItr->second;
        if (oldPlr)
        {
            oldPlr->Kick(L"Not In Server Whitelist !");
        }
    }
    if(IsWhitelisted(playerName))
    {
        whitelist.erase(playerName);
        std::ofstream whitelistFile;
        std::string stringPlayerName;
        whitelistFile.open(whitelistFileName.c_str(), std::fstream::out | std::fstream::trunc);
        for (auto itrPlr = whitelist.begin(); itrPlr != whitelist.end(); itrPlr++)
        {
            Util::WStringToString(*itrPlr, stringPlayerName);
            whitelistFile << stringPlayerName << std::endl;
        }
        whitelistFile.close();
        return true;
    }
    return false;
}

bool WorldManager::Mute(const std::wstring& playerName)
{
    if (!IsMuted(playerName))
    {
        mutedPlayers.insert(playerName);
        std::ofstream mutedPlayersFile;
        std::string stringPlayerName;
        Util::WStringToString(playerName, stringPlayerName);
        mutedPlayersFile.open(mutedPlayersFileName.c_str(), std::fstream::out | std::fstream::app);
        mutedPlayersFile << stringPlayerName << std::endl;
        mutedPlayersFile.close();
        
        EntityPlayer* player = GetPlayerByName(playerName);
        if (player != nullptr)
        {
            player->GetChat().Mute(true);
        }
        return true;
    }
    return false;
}

bool WorldManager::UnMute(const std::wstring& playerName)
{
    if (IsMuted(playerName))
    {
        mutedPlayers.erase(playerName);
        std::ofstream mutedPlayersFile;
        std::string stringPlayerName;
        mutedPlayersFile.open(mutedPlayersFileName.c_str(), std::fstream::out | std::fstream::trunc);
        for (auto itrPlr = mutedPlayers.begin(); itrPlr != mutedPlayers.end(); itrPlr++)
        {
            Util::WStringToString(*itrPlr, stringPlayerName);
            mutedPlayersFile << stringPlayerName << std::endl;
        }
        mutedPlayersFile.close();
        
        EntityPlayer* player = GetPlayerByName(playerName);
        if (player != nullptr)
        {
            player->GetChat().Mute(false);
        }
        return true;
    }
    return false;
}

void WorldManager::Reload()
{
    loadBanList();
    loadAdminList();
    if(useWhitelist)
    {
        loadWhitelist();
    }
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
    libconfig::Setting& setting = Config::Config::GetConfig().lookup("server.general");
    if(setting["motd"].isArray())
    {
        motdArraySize = setting["motd"].getLength();
        serverMotd = new std::string[motdArraySize];
        for(int i = 0; i < motdArraySize; i++)
        {
            serverMotd[i] = setting["motd"][i].c_str();
        }
    }
}

void WorldManager::loadWhitelist()
{
    whitelist.clear();
    std::ifstream whitelistFile(whitelistFileName.c_str());
    std::string line;
    while (std::getline(whitelistFile,line))
    {
        std::wstring playerName;
        Util::StringToWString(playerName, line);
        whitelist.insert(playerName);
    }
    whitelistFile.close();
}

void WorldManager::loadMutedPlayersList()
{
    mutedPlayers.clear();
    std::ifstream mutedPlayersFile(mutedPlayersFileName.c_str());
    std::string line;
    while (std::getline(mutedPlayersFile,line))
    {
        std::wstring playerName;
        Util::StringToWString(playerName, line);
        mutedPlayers.insert(playerName);
    }
    mutedPlayersFile.close();
}

bool WorldManager::IsAdmin(const std::wstring& playerName)
{
    return (adminList.find(playerName) != adminList.end());
}

bool WorldManager::IsBan(const std::wstring& playerName)
{
    return (banList.find(playerName) != banList.end());
}

bool WorldManager::IsWhitelisted(const std::wstring& playerName)
{
    if(!useWhitelist)
    {
        return true;
    }
    return (whitelist.find(playerName) != whitelist.end());
}

bool WorldManager::IsMuted(const std::wstring& playerName)
{
    return (mutedPlayers.find(playerName) != mutedPlayers.end());
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
