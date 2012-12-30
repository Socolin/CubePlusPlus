#include "WorldManager.h"

#include <iostream>

#include "World.h"
#include "Entity/EntityPlayer.h"

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
    delete player;
}

bool WorldManager::IsRunning()
{
    return isRunning;
}

WorldManager::WorldManager() :
        world(NULL), isRunning(true)
{

}

WorldManager::~WorldManager()
{
    world->Unload();
    delete world;
    world = NULL;
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

} /* namespace World */
