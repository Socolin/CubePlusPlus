#include <Network/NetworkManager.h>
#include <Network/Opcode.h>
#include "World/WorldManager.h"
#include <iostream>

int main(void)

{
    Network::initOpcode();
    Network::NetworkManager manager;
    World::WorldManager::GetInstance()->Init();
    if (!manager.StartServer(25565))
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

