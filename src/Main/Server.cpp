#include <Network/NetworkManager.h>
#include <Network/Opcode.h>

#include "Config/Config.h"
#include "Craft/CraftManager.h"
#include "Block/BlockList.h"
#include "Block/TileEntities/RegisterTileEntities.h"
#include "Inventory/ItemList.h"
#include "Database/DatabaseManager.h"
#include "Database/MiscData/PaintingData.h"
#include "Database/MiscData/FurnaceRecipes.h"
#include "Scripting/ScriptManager.h"
#include "Window/WindowList.h"
#include "World/WorldManager.h"
#include "World/World.h"

#include <iostream>
#include <ctime>

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

    // Load data
    Block::BlockList::Instance().InitInstance();
    Inventory::ItemList::Instance().InitInstance();
    Window::WindowList::Instance().InitInstance();
    Craft::CraftManager::Instance().InitInstance();
    Database::PaintingData::Instance().InitInstance();
    Database::FurnaceRecipes::Instance().InitInstance();
    Block::RegisterTileEntities();

    // Tick time management
    clock_t time = std::clock();
    struct timespec requestTime;
    struct timespec unused;
    requestTime.tv_sec = 0;

    long lateness = 0;
    while (worldManager->IsRunning())
    {
        manager.ReceiveData();
        worldManager->UpdateTick();
        clock_t diff = std::clock() - time;
        time = std::clock();

        long diffInMsSeconds = diff * 1000;
        if ((diffInMsSeconds + lateness) < 50 * CLOCKS_PER_SEC)
        {
            lateness = 0;
            requestTime.tv_nsec = ((50 * CLOCKS_PER_SEC) - (diffInMsSeconds + lateness));
            nanosleep(&requestTime, &unused);
        }
        else
        {
            lateness = ((diffInMsSeconds + lateness) - (50 * CLOCKS_PER_SEC));
            std::cerr << "Tick take more than 50ms: "<< diffInMsSeconds / CLOCKS_PER_SEC << "ms, (lateness:" << lateness / CLOCKS_PER_SEC << "ms)" << std::endl;
            usleep(1);
        }
    }
    Scripting::ScriptManager::DeleteInstance();
    delete worldManager;
    return 0;
}

