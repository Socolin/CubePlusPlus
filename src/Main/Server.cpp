#include <Network/NetworkManager.h>
#include <Network/Opcode.h>
#include "World/WorldManager.h"
#include "Config/Config.h"
#include <iostream>

int main(void)

{
    Network::initOpcode();
    Network::NetworkManager manager;
    World::WorldManager::GetInstance()->Init();
    int port = 25565;

    if((Config::Config::getConfig()).lookupValue("server.network.port", port))
    {
    	std::cout << "Custom port " << port << " detected" << std::endl;
    }

    if (!manager.StartServer(port))
    {
        std::cerr << "Not started" << std::endl;
        return 1;
    }
    while (1)
    {
        manager.ReceiveData();
        World::WorldManager::GetInstance()->UpdateTick();
        usleep(1);
    }
    return 0;
}

