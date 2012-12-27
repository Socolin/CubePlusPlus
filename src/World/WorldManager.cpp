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

    return player;
}

void WorldManager::RemovePlayer(EntityPlayer* player)
{
    world->RemovePlayer(player);
}

WorldManager::WorldManager() :
        world(NULL)
{

}

WorldManager::~WorldManager()
{
}

void WorldManager::UpdateTick() const
{
    world->UpdateTick();
}

void WorldManager::Init()
{
    world = new World();
}

} /* namespace World */
