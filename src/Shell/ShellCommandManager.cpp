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
        std::cout << ">> Stopping the server..." << std::endl;
        World::WorldManager::Instance().Stop();
    }
    else if (message.substr(0, 6) == L"reload")
    {
        std::cout << ">> Reloading VIP list, Admin list and Whitelist" << std::endl;
        World::WorldManager::Instance().Reload();
    }
    else if (message.substr(0, 5) == L"night")
    {
        std::cout << ">> You switch time to night" << std::endl;
        World::WorldManager::Instance().GetWorld()->SetTime(20000);
    }
    else if (message.substr(0, 3) == L"day")
    {
        std::cout << ">> You switch time to day" << std::endl;
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
            std::cout << ">> Not a valid player name" << std::endl;
        }
    }
    else if (message.substr(0, 4) == L"ban ")
    {
        std::wstring playerName = message.substr(4, message.size() - 5);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().Ban(playerName))
            {
                std::wcout << L">> Player "<< playerName.c_str() << L" banned" << std::endl;
            }
            else
            {
                std::wcout << L">> Player "<< playerName.c_str() << L" is already banned" << std::endl;
            }
        }
        else
        {
            std::cout << ">> Not a valid player name" << std::endl;
        }
    }
    else if (message.substr(0, 6) == L"unban ")
    {
        std::wstring playerName = message.substr(6, message.size() - 7);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().UnBan(playerName))
            {
                std::wcout << L">> Player "<< playerName.c_str() << L" unbanned" << std::endl;
            }
            else
            {
                std::wcout << L">> Player "<< playerName.c_str() << L" is not banned" << std::endl;
            }
        }
        else
        {
            std::cout << ">> Not a valid player name" << std::endl;
        }
    }
    else if (message.substr(0, 9) == L"addadmin ")
    {
        std::wstring playerName = message.substr(9, message.size() - 10);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().SetAdmin(playerName))
            {
                std::wcout << L">> Player "<< playerName.c_str() << L" is now admin" << std::endl;
            }
            else
            {
                std::wcout << L">> Player "<< playerName.c_str() << L" is already admin" << std::endl;
            }
        }
        else
        {
            std::cout << ">> Not a valid player name" << std::endl;
        }
    }
    else if (message.substr(0, 8) == L"unadmin ")
    {
        std::wstring playerName = message.substr(8, message.size() - 9);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().UnAdmin(playerName))
            {
                std::wcout << L">> Player "<< playerName.c_str() << L" is no longer admin" << std::endl;
            }
            else
            {
                std::wcout << L">> Player "<< playerName.c_str() << L" is not admin" << std::endl;
            }
        }
        else
        {
            std::cout << ">> Not a valid player name" << std::endl;
        }
    }
    else if (message.substr(0, 10) == L"whitelist ")
    {
        std::wstring playerName = message.substr(10, message.size() - 11);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().AddToWhitelist(playerName))
            {
                std::wcout << L">> Player "<< playerName.c_str() << L" added to whitelist" << std::endl;
            }
            else
            {
                std::wcout << L">> Player "<< playerName.c_str() << L" is already in whitelist" << std::endl;
            }
        }
        else
        {
            std::cout << ">> Not a valid player name" << std::endl;
        }
    }
    else if (message.substr(0, 12) == L"unwhitelist ")
    {
        std::wstring playerName = message.substr(12, message.size() - 13);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().UnWhitelist(playerName))
            {
                std::wcout << L">> Player "<< playerName.c_str() << L" is no longer in whitelist" << std::endl;
            }
            else
            {
                std::wcout << L">> Player "<< playerName.c_str() << L" is not in whitelist" << std::endl;
            }
        }
        else
        {
            std::cout << ">> Not a valid player name" << std::endl;
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
        std::cout << ">> Not a valid command" << std::endl;
    }
}

} /* namespace Shell */
