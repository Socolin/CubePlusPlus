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
#include "Logging/Logger.h"
#include "Network/LoginManager.h"
#include "Scripting/ScriptManager.h"
#include "Window/WindowList.h"
#include "World/WorldManager.h"
#include "World/World.h"

#include <iostream>
#include <ctime>
#include <signal.h>


void stopHandler(int sig)
{
    World::WorldManager::Instance().Stop();
    signal(sig, stopHandler);
}


int main(void)
{
    signal(SIGINT, stopHandler);
    signal(SIGTSTP, stopHandler);

    Database::DatabaseManager::InitInstance();
    Network::initOpcode();
    Network::NetworkManager manager;
    World::WorldManager& worldManager = World::WorldManager::Instance();
    worldManager.Init();
    int port = 25565;

    Scripting::ScriptManager::Instance().RegisterAllScripts();
    if((Config::Config::getConfig()).lookupValue("server.network.port", port))
    {
        LOG_INFO << "Custom port " << port << " detected" << std::endl;
    }

    if (!manager.StartServer(port))
    {
        LOG_ERROR << "Not started" << std::endl;
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

    Network::LoginManager::Instance().Start();

    // Tick time management
    clock_t time = std::clock();
    struct timespec requestTime;
    struct timespec unused;
    requestTime.tv_sec = 0;

    long long lateness = 0;
    while (worldManager.IsRunning())
    {
        manager.ReceiveData();
        worldManager.UpdateTick();
        clock_t diff = std::clock() - time;
        time = std::clock();

        long long diffInMsSeconds = diff * 1000;
        if ((diffInMsSeconds + lateness) < 50 * CLOCKS_PER_SEC)
        {
            lateness = 0;
            requestTime.tv_nsec = ((50 * CLOCKS_PER_SEC) - (diffInMsSeconds + lateness));
            nanosleep(&requestTime, &unused);
        }
        else
        {
            lateness = ((diffInMsSeconds + lateness) - (50 * CLOCKS_PER_SEC));
            if (diffInMsSeconds >= 50 * CLOCKS_PER_SEC)
                LOG_ERROR << "Tick take more than 50ms: "<< diffInMsSeconds / CLOCKS_PER_SEC << "ms, (lateness:" << lateness / CLOCKS_PER_SEC << "ms)" << std::endl;
            usleep(1);
        }
        worldManager.SetLateness(lateness);
    }

    Network::LoginManager::Instance().Stop();
    return 0;
}

