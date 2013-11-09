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
        LOG_INFO << "Stopping the server..." << std::endl;
        World::WorldManager::Instance().Stop();
    }
    else if (message.substr(0, 6) == L"reload")
    {
        LOG_INFO << "Reloading Ban list, Admin list and Whitelist" << std::endl;
        World::WorldManager::Instance().Reload();
    }
    else if (message.substr(0, 5) == L"night")
    {
        LOG_INFO << "You switch time to night" << std::endl;
        World::WorldManager::Instance().GetWorld()->SetTime(20000);
    }
    else if (message.substr(0, 3) == L"day")
    {
        LOG_INFO << "You switch time to day" << std::endl;
        World::WorldManager::Instance().GetWorld()->SetTime(1000);
    }
    else if (message.substr(0, 5) == L"kick ")
    {
        std::wstring playerName = message.substr(5, message.size() - 6);
        if (playerName.size() > 1)
        {
            World::WorldManager::Instance().Kick(playerName);
            LOG_INFO << "Player "<< playerName.c_str() << " kicked" << std::endl;
        }
        else
        {
            LOG_ERROR << "Not a valid player name" << std::endl;
        }
    }
    else if (message.substr(0, 4) == L"ban ")
    {
        std::wstring playerName = message.substr(4, message.size() - 5);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().Ban(playerName))
            {
                LOG_INFO << "Player "<< playerName.c_str() << " banned" << std::endl;
            }
            else
            {
                LOG_ERROR << "Player "<< playerName.c_str() << " is already banned" << std::endl;
            }
        }
        else
        {
            LOG_ERROR << "Not a valid player name" << std::endl;
        }
    }
    else if (message.substr(0, 6) == L"unban ")
    {
        std::wstring playerName = message.substr(6, message.size() - 7);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().UnBan(playerName))
            {
                LOG_INFO << "Player "<< playerName.c_str() << " unbanned" << std::endl;
            }
            else
            {
                LOG_ERROR << "Player "<< playerName.c_str() << " is not banned" << std::endl;
            }
        }
        else
        {
            LOG_ERROR << "Not a valid player name" << std::endl;
        }
    }
    else if (message.substr(0, 9) == L"addadmin ")
    {
        std::wstring playerName = message.substr(9, message.size() - 10);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().SetAdmin(playerName))
            {
                LOG_INFO << "Player "<< playerName.c_str() << " is now admin" << std::endl;
            }
            else
            {
                LOG_ERROR << "Player "<< playerName.c_str() << " is already admin" << std::endl;
            }
        }
        else
        {
            LOG_ERROR << "Not a valid player name" << std::endl;
        }
    }
    else if (message.substr(0, 8) == L"unadmin ")
    {
        std::wstring playerName = message.substr(8, message.size() - 9);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().UnAdmin(playerName))
            {
                LOG_INFO << "Player "<< playerName.c_str() << " is no longer admin" << std::endl;
            }
            else
            {
                LOG_ERROR << "Player "<< playerName.c_str() << " is not admin" << std::endl;
            }
        }
        else
        {
            LOG_ERROR << "Not a valid player name" << std::endl;
        }
    }
    else if (message.substr(0, 10) == L"whitelist ")
    {
        std::wstring playerName = message.substr(10, message.size() - 11);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().AddToWhitelist(playerName))
            {
                LOG_INFO << "Player "<< playerName.c_str() << " added to whitelist" << std::endl;
            }
            else
            {
                LOG_ERROR << "Player "<< playerName.c_str() << " is already in whitelist" << std::endl;
            }
        }
        else
        {
            LOG_ERROR << "Not a valid player name" << std::endl;
        }
    }
    else if (message.substr(0, 12) == L"unwhitelist ")
    {
        std::wstring playerName = message.substr(12, message.size() - 13);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().UnWhitelist(playerName))
            {
                LOG_INFO << "Player "<< playerName.c_str() << " is no longer in whitelist" << std::endl;
            }
            else
            {
                LOG_ERROR << "Player "<< playerName.c_str() << " is not in whitelist" << std::endl;
            }
        }
        else
        {
            LOG_ERROR << "Not a valid player name" << std::endl;
        }
    }
    else if (message.substr(0,11) == L"playercount")
    {
        LOG_INFO << "Connected players : " << World::WorldManager::Instance().GetPlayerCount() << std::endl;
    }
    else if (message.substr(0,10) == L"playerlist")
    {
        std::map<std::wstring, World::EntityPlayer*>* playerList = World::WorldManager::Instance().GetPlayerByNameList();
        LOG_INFO << "Connected players : " << std::endl;
        for(auto itr : *playerList)
        {
            LOG_INFO_CONCAT << itr.first << " - ";
        }
        LOG_INFO_CONCAT << "Total : " << World::WorldManager::Instance().GetPlayerCount() << " players connected" << std::endl;
    }
    else
    {
        LOG_ERROR << "Not a valid command" << std::endl;
    }
}

} /* namespace Shell */
