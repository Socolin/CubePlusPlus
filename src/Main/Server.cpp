#include <Network/NetworkManager.h>
#include <Network/Opcode.h>
#include "World/WorldManager.h"
#include "Config/Config.h"
#include "Block/BlockList.h"
#include "Inventory/ItemList.h"
#include "Database/DatabaseManager.h"
#include "Scripting/ScriptManager.h"
#include <iostream>

int main(void)

{
    Database::DatabaseManager::InitInstance();
    Network::initOpcode();
    Network::NetworkManager manager;
    World::WorldManager* worldManager = World::WorldManager::GetInstance();
    worldManager->Init();
    int port = 25565;

    Scripting::ScriptManager::GetInstance()->RegisterAllScripts();
    if((Config::Config::getConfig()).lookupValue("server.network.port", port))
    {
    	std::cout << "Custom port " << port << " detected" << std::endl;
    }

    if (!manager.StartServer(port))
    {
        std::cerr << "Not started" << std::endl;
        return 1;
    }
    Block::BlockList::InitInstance();
    Inventory::ItemList::InitInstance();

    while (worldManager->IsRunning())
    {
        manager.ReceiveData();
        worldManager->UpdateTick();
        usleep(1);
    }
    delete worldManager;
    return 0;
}

