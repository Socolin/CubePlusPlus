#include "WorldManager.h"

#include <iostream>

#include "Entity/EntityPlayer.h"
#include "Network/NetworkPacket.h"
#include "Network/OpcodeList.h"
#include "World.h"

namespace World
{
WorldManager* WorldManager::instance = NULL;

WorldManager* WorldManager::GetInstance()
{
    if (instance == NULL)
        instance = new WorldManager();
    return instance;
}

EntityPlayer* WorldManager::LoadAndJoinWorld(const std::wstring& name, Network::NetworkSession* session)
{
    EntityPlayer* player = new EntityPlayer(0, 100, 0, name, session);

    world->AddPlayer(player);

    playerList.insert(player);
    return player;
}

void WorldManager::RemovePlayer(EntityPlayer* player)
{
    world->RemovePlayer(player);
    playerList.erase(player);
    // delete player; do it later
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

int WorldManager::getPlayerCount()
{
    return playerList.size();
}

WorldManager::WorldManager() :
    world(nullptr), isRunning(true)
{

}

World* WorldManager::GetWorld()
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
    world = new World();
}
void WorldManager::Stop()
{
    isRunning = false;
    for (auto itrPlr = playerList.begin(); itrPlr != playerList.end(); )
    {
        EntityPlayer* toKick = *itrPlr;
        itrPlr++;
        toKick->Kick();
    }
}
void WorldManager::SendToAllPlayer(Network::NetworkPacket& packet)
{
for (EntityPlayer* plr : playerList)
    {
        plr->Send(packet);
    }
}

} /* namespace World */
