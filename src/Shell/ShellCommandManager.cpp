#include "ShellCommandManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

#include "Entity/EntityPlayer.h"
#include "Logging/Logger.h"
#include "World/WorldManager.h"
#include "World/World.h"

#define STDIN 0

namespace Shell
{

ShellCommandManager::ShellCommandManager():
        retval(0)
{
    tv.tv_sec = 0;
    tv.tv_usec = 0;
}

ShellCommandManager::~ShellCommandManager()
{
}

void ShellCommandManager::HandleShellInput()
{
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    retval = select(1, &rfds, nullptr, nullptr, &tv);
    if (retval == -1)
    {
        perror("select()");
    }
    else if (retval)
    {
        if(FD_ISSET(STDIN, &rfds))
        {
            retval = read(0, buffer, sizeof buffer);
            std::wostringstream message;
            message << buffer;
            HandleShellCommand(message.str());
            FD_CLR(STDIN, &rfds);
            memset(buffer, 0, 255);
        }
    }
}

void ShellCommandManager::HandleShellCommand(std::wstring message)
{
    if(message.size() > 0)
    {
        message[0] = tolower(message[0]);
    }

    if (message.substr(0, 4) == L"stop")
    {
        printf(">> Stopping the server...\n");
        World::WorldManager::Instance().Stop();
    }
    else if (message.substr(0, 6) == L"reload")
    {
        printf(">> Reloading VIP and Admin list\n");
        World::WorldManager::Instance().Reload();
    }
    else if (message.substr(0, 5) == L"night")
    {
        printf(">> You switch time to night\n");
        World::WorldManager::Instance().GetWorld()->SetTime(20000);
    }
    else if (message.substr(0, 3) == L"day")
    {
        printf(">> You switch time to day\n");
        World::WorldManager::Instance().GetWorld()->SetTime(1000);
    }
    else if (message.substr(0, 5) == L"kick ")
    {
        std::wstring playerName = message.substr(5, message.size() - 6);
        if (playerName.size() > 1)
        {
            World::WorldManager::Instance().Kick(playerName);
            std::wcout << L">> Player "<< playerName.c_str() << L" kicked" << std::endl;
        }
        else
        {
            printf(">> Not a valid player name\n");
        }
    }
    else if (message.substr(0, 4) == L"ban ")
    {
        std::wstring playerName = message.substr(4, message.size() - 5);
        if (playerName.size() > 0)
        {
            World::WorldManager::Instance().Ban(playerName);
            std::wcout << L">> Player "<< playerName.c_str() << L" banned" << std::endl;
        }
        else
        {
            printf(">> Not a valid player name\n");
        }
    }
    else if (message.substr(0, 9) == L"addadmin ")
    {
        std::wstring playerName = message.substr(9, message.size() - 10);
        if (playerName.size() > 0)
        {
            World::WorldManager::Instance().SetAdmin(playerName);
            std::wcout << L">> Player "<< playerName.c_str() << L" is now admin" << std::endl;
        }
        else
        {
            printf(">> Not a valid player name\n");
        }
    }
    else if (message.substr(0,11) == L"playercount")
    {
        LOG_INFO << L"Connected players : " << World::WorldManager::Instance().GetPlayerCount() << std::endl;
    }
    else if (message.substr(0,10) == L"playerlist")
    {
        std::map<std::wstring, World::EntityPlayer*>* playerList = World::WorldManager::Instance().GetPlayerByNameList();
        LOG_INFO << L"Connected players : " << std::endl;
        for(auto itr : *playerList)
        {
            LOG_INFO_CONCAT << itr.first << L" - ";
        }
        LOG_INFO_CONCAT << L"Total : " << World::WorldManager::Instance().GetPlayerCount() << L" players connected" << std::endl;
    }
    else
    {
        printf(">> Not a valid command\n");
    }
}

} /* namespace Shell */
