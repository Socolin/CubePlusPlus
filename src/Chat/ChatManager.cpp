#include "ChatManager.h"

#include <iostream>
#include <sstream>

#include "Config/Config.h"
#include "Entity/EntityPlayer.h"
#include "Entity/Scripts/Database/ScriptedEntityList.h"
#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Logging/Logger.h"
#include "Util/StringUtil.h"
#include "World/WorldManager.h"
#include "World/World.h"

#include <boost/algorithm/string.hpp>

namespace Chat
{

ChatManager::ChatManager()
    : fwFileName("forbiddenWords")
{
    Config::Config::GetConfig().lookupValue("server.general.forbidden-words-file", fwFileName);
    loadForbiddenWordsList();
}

ChatManager::~ChatManager()
{
}

bool ChatManager::HandleChatMessage(World::EntityPlayer* player, std::wstring& message)
{
    if (message.substr(0, 1) == L"/")
    {
        bool commandHandled = false;
        if (player->isAdmin())
            commandHandled |= handleAdminCommand(player, message);

        if (!commandHandled)
        {
            player->GetChat() << Chat::RED << "Not a valid command" << std::endl;;
        }
        return true;
    }

    std::set<std::wstring>::iterator it;
    std::wstring messageLow = message;
    boost::algorithm::to_lower(messageLow);
    for(it = forbiddenWords.begin(); it != forbiddenWords.end(); ++it)
    {
        if(messageLow.find(*it) != messageLow.npos)
        {
            player->GetChat() << Chat::RED << "Your message include forbidden words and will not be send";
            LOG_INFO << "Invalid message send by player " << player->GetUsername() << " : " << message << std::endl;
            return true;
        }
    }

    std::wcout << player->GetUsername() << ":" << message << std::endl;

    std::wostringstream formatedMessage;
    formatedMessage << L"<" << player->GetUsername() << L"> " << message;
    message = formatedMessage.str();
    LOG_INFO << message << std::endl;
    return false;
}

bool ChatManager::handleAdminCommand(World::EntityPlayer* player, std::wstring& message)
{
    if (message == L"/stop")
    {
        player->SendChatMessage(L"§aStopping the server");
        World::WorldManager::Instance().Stop();
    }
    else if (message == L"/reload")
    {
        player->SendChatMessage(L"§aReloading Ban list, Admin list and Whitelist");
        World::WorldManager::Instance().Reload();
    }
    else if (message == L"/save")
    {
        player->SendChatMessage(L"§aSaving world");
        player->GetWorld()->Save();
    }
    else if (message == L"/night")
    {
        player->SendChatMessage(L"§aYou switch time to night");
        World::WorldManager::Instance().GetWorld()->SetTime(20000);
    }
    else if (message == L"/day")
    {
        player->SendChatMessage(L"§aYou switch time to day");
        World::WorldManager::Instance().GetWorld()->SetTime(1000);
    }
    else if (message.substr(0, 4) == L"/tp ")
    {
        std::wstringstream ss(message.substr(4));
        double x;
        double y;
        double z;
        if (ss >> x >> y >> z)
        {
            player->Teleport(x, y, z, player->GetYaw(), player->GetPitch());
        }
        else
        {
            std::wstring playerNameSrc;
            std::wstring playerNameDest;
            std::vector<std::wstring> tpPlayers = Util::split(message.substr(4), ' ');
            if(1 == tpPlayers.size())
            {
                playerNameDest = tpPlayers[0];
                World::EntityPlayer* destPlayer = World::WorldManager::Instance().GetPlayerByName(playerNameDest);
                if (nullptr != destPlayer)
                {
                    player->Teleport(destPlayer->x, destPlayer->y, destPlayer->z, destPlayer->GetYaw(), destPlayer->GetPitch());
                    player->GetChat() << Chat::GREEN << L"Teleported to " << playerNameDest << std::endl;
                }
                else
                {
                    player->GetChat() << Chat::RED << L"Can't teleport to " << playerNameDest << L", player not found in current map" << std::endl;
                    displayMatchingPrefix(player, playerNameDest);
                }
            }
            else if(2 == tpPlayers.size())
            {
                playerNameSrc = tpPlayers[0];
                playerNameDest = tpPlayers[1];
                World::EntityPlayer* srcPlayer = World::WorldManager::Instance().GetPlayerByName(playerNameSrc);
                World::EntityPlayer* destPlayer = World::WorldManager::Instance().GetPlayerByName(playerNameDest);
                if (nullptr != srcPlayer && nullptr != destPlayer)
                {
                    srcPlayer->Teleport(destPlayer->x, destPlayer->y, destPlayer->z, destPlayer->GetYaw(), destPlayer->GetPitch());
                    player->GetChat() << Chat::GREEN << playerNameSrc << L" teleported to " << playerNameDest << std::endl;
                }
                else if (nullptr == srcPlayer)
                {
                    player->GetChat() << Chat::RED << L"Can't find player " << playerNameSrc << L" in current map" << std::endl;
                    displayMatchingPrefix(player, playerNameSrc);
                }
                else if (nullptr == destPlayer)
                {
                    player->GetChat() << Chat::RED << L"Can't find player " << playerNameDest << L" in current map" << std::endl;
                    displayMatchingPrefix(player, playerNameDest);
                }
            }
            else
            {
                player->GetChat() << Chat::RED << L"Invalid arguments" << std::endl;
            }
        }
    }
    else if (message.substr(0, 6) == L"/kick ")
    {
        std::wstring playerName = message.substr(6, message.size() - 6);
        if (playerName.size() > 0)
        {
            World::WorldManager::Instance().Kick(playerName);
            std::wostringstream confirmMessage;
            player->GetChat() << Chat::GREEN << playerName << L" kicked";
        }
    }
    else if (message.substr(0, 5) == L"/ban ")
    {
        std::wstring playerName = message.substr(5, message.size() - 5);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().Ban(playerName))
            {
                player->GetChat() << Chat::GREEN << playerName << L" banned" << std::endl;
            }
            else
            {
                player->GetChat() << Chat::RED << L"Can't ban " << playerName << std::endl;
            }
        }
    }
    else if (message.substr(0, 7) == L"/unban ")
    {
        std::wstring playerName = message.substr(7, message.size() - 7);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().UnBan(playerName))
            {
                player->GetChat() << Chat::GREEN << playerName << L" unbanned" << std::endl;
            }
            else
            {
                player->GetChat() << Chat::RED << playerName << L" is not banned" << std::endl;
            }
        }
    }
    else if (message.substr(0, 10) == L"/addadmin ")
    {
        std::wstring playerName = message.substr(10, message.size() - 10);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().SetAdmin(playerName))
            {
                player->GetChat() << Chat::GREEN << playerName << L" is now admin" << std::endl;
            }
            else
            {
                player->GetChat() << Chat::RED << playerName << L" is already admin" << std::endl;
            }
        }
    }
    else if (message.substr(0, 9) == L"/unadmin ")
    {
        std::wstring playerName = message.substr(9, message.size() - 9);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().UnAdmin(playerName))
            {
                player->GetChat() << Chat::GREEN << playerName << L" is no longer admin" << std::endl;
            }
            else
            {
                player->GetChat() << Chat::RED << playerName << L" is not admin" << std::endl;
            }
        }
    }
    else if (message.substr(0, 11) == L"/whitelist ")
    {
        std::wstring playerName = message.substr(11, message.size() - 11);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().AddToWhitelist(playerName))
            {
                player->GetChat() << Chat::GREEN << playerName << L" added to whitelist" << std::endl;
            }
            else
            {
                player->GetChat() << Chat::RED << playerName << L" is already in whitelist" << std::endl;
            }
        }
    }
    else if (message.substr(0, 13) == L"/unwhitelist ")
    {
        std::wstring playerName = message.substr(13, message.size() - 13);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().UnWhitelist(playerName))
            {
                player->GetChat() << Chat::GREEN << playerName << L" is no longer in whitelist" << std::endl;
            }
            else
            {
                player->GetChat() << Chat::RED << playerName << L" is not in whitelist" << std::endl;
            }
        }
    }
    else if (message.substr(0, 9) == L"/addword ")
    {
        std::wstring word = message.substr(9, message.size() - 9);
        if (word.size() > 0)
        {
            if(AddForbiddenWord(word))
            {
                player->GetChat() << Chat::GREEN << word << L" added to forbidden word list" << std::endl;
            }
            else
            {
                player->GetChat() << Chat::RED << word << L" is already in forbidden word list" << std::endl;
            }
        }
    }
    else if (message.substr(0, 8) == L"/unword ")
    {
        std::wstring word = message.substr(8, message.size() - 8);
        if (word.size() > 0)
        {
            if(RemoveForbiddenWord(word))
            {
                player->GetChat() << Chat::GREEN << word << L" is no longer in forbidden word list" << std::endl;
            }
            else
            {
                player->GetChat() << Chat::RED << word << L" is not in forbidden word list" << std::endl;
            }
        }
    }
    else if (message == L"/spawn chicken")
    {
        World::ScriptedLivingEntity* entity = World::ScriptedEntityList::Instance().CreateNewEntity(1, player->x, player->y, player->z);
        entity->Rotate(player->GetYaw(), player->GetPitch());
        player->GetWorld()->AddEntity(entity);
    }
    else if (message == L"/spawn cow")
    {
        World::ScriptedLivingEntity* entity = World::ScriptedEntityList::Instance().CreateNewEntity(2, player->x, player->y, player->z);
        entity->Rotate(player->GetYaw(), player->GetPitch());
        player->GetWorld()->AddEntity(entity);
    }
    else if (message == L"/spawn pig")
    {
        World::ScriptedLivingEntity* entity = World::ScriptedEntityList::Instance().CreateNewEntity(3, player->x, player->y, player->z);
        entity->Rotate(player->GetYaw(), player->GetPitch());
        player->GetWorld()->AddEntity(entity);
    }
    else if (message == L"/spawn sheep")
    {
        World::ScriptedLivingEntity* entity = World::ScriptedEntityList::Instance().CreateNewEntity(4, player->x, player->y, player->z);
        entity->Rotate(player->GetYaw(), player->GetPitch());
        player->GetWorld()->AddEntity(entity);
    }
    else if (message == L"/spawn wolf")
    {
        World::ScriptedLivingEntity* entity = World::ScriptedEntityList::Instance().CreateNewEntity(5, player->x, player->y, player->z);
        entity->Rotate(player->GetYaw(), player->GetPitch());
        player->GetWorld()->AddEntity(entity);
    }
    else if (message == L"/spawn zombie")
    {
        World::ScriptedLivingEntity* entity = World::ScriptedEntityList::Instance().CreateNewEntity(6, player->x, player->y, player->z);
        entity->Rotate(player->GetYaw(), player->GetPitch());
        player->GetWorld()->AddEntity(entity);
    }
    else
    {
        return false;
    }

    return true;
}

void ChatManager::loadForbiddenWordsList()
{
    forbiddenWords.clear();
    std::ifstream forbiddenWordsList(fwFileName.c_str());
    std::string line;
    while (std::getline(forbiddenWordsList, line))
    {
        std::wstring forbiddenWord;
        Util::StringToWString(forbiddenWord, line);
        boost::algorithm::to_lower(forbiddenWord);
        forbiddenWords.insert(forbiddenWord);
    }
    forbiddenWordsList.close();
}

bool ChatManager::AddForbiddenWord(const std::wstring& word)
{
    std::wstring forbiddenWord = word;
    boost::algorithm::to_lower(forbiddenWord);
    if (!IsForbiddenWord(forbiddenWord))
    {
        forbiddenWords.insert(forbiddenWord);
        std::ofstream forbiddenWordsList;
        std::string stringWord;
        Util::WStringToString(forbiddenWord, stringWord);
        forbiddenWordsList.open(fwFileName.c_str(), std::fstream::out | std::fstream::app);
        forbiddenWordsList << stringWord << std::endl;
        forbiddenWordsList.close();
        return true;
    }
    return false;

}

bool ChatManager::RemoveForbiddenWord(const std::wstring& word)
{
    std::wstring forbiddenWord = word;
    boost::algorithm::to_lower(forbiddenWord);
    if (IsForbiddenWord(forbiddenWord))
    {
        forbiddenWords.erase(forbiddenWord);
        std::ofstream forbiddenWordsListFile;
        std::string stringWord;
        forbiddenWordsListFile.open(fwFileName.c_str(), std::fstream::out | std::fstream::trunc);
        for (auto itrPlr = forbiddenWords.begin(); itrPlr != forbiddenWords.end(); itrPlr++)
        {
            Util::WStringToString(*itrPlr, stringWord);
            forbiddenWordsListFile << stringWord << std::endl;
        }
        forbiddenWordsListFile.close();
        return true;
    }
    return false;
}

bool ChatManager::IsForbiddenWord(const std::wstring& word)
{
    return (forbiddenWords.find(word) != forbiddenWords.end());
}

void ChatManager::displayMatchingPrefix(World::EntityPlayer* player, const std::wstring& prefix)
{
    std::map<std::wstring, World::EntityPlayer*>* playerList = World::WorldManager::Instance().GetPlayerByNameList();
    player->GetChat() << Chat::BLUE << L"Connected players with a similar name : " << std::endl;
    for(auto playerItr = playerList->begin(); playerItr != playerList->end(); playerItr++)
    {
        if (prefix == (playerItr->first).substr(0, prefix.size()))
        {
            player->GetChat() << Chat::BLUE << L"   - " << playerItr->first << std::endl;
        }
    }
}


} /* namespace Chat */
