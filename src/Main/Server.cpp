#include <Network/NetworkManager.h>
#include <Network/Opcode.h>

#include "Config/Config.h"
#include "Craft/CraftManager.h"
#include "Block/BlockList.h"
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

    // Tick time management
    clock_t time = std::clock();
    struct timespec requestTime;
    struct timespec unused;
    requestTime.tv_sec = 0;

    worldManager->GetWorld()->GetChunk(0,0);
    while (worldManager->IsRunning())
    {
        time = std::clock();
        manager.ReceiveData();
        worldManager->UpdateTick();
        clock_t diff = std::clock() - time;

        double diffInSeconds = (((double)diff)/CLOCKS_PER_SEC);
        if (diffInSeconds < 0.05)
        {
            requestTime.tv_nsec = (0.05 - diffInSeconds) * 1000000000;
            nanosleep(&requestTime, &unused);
            //std::cout << diffInSeconds << std::endl;;
        }
        else
        {
            std::cerr << "Tick take more than 5ms :"<< diffInSeconds << "seconds" << std::endl;
            usleep(1);
        }
    }

    delete worldManager;
    return 0;
}

