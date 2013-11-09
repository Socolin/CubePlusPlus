#include "ChatManager.h"

#include <iostream>
#include <sstream>

#include "Config/Config.h"
#include "Entity/EntityPlayer.h"
#include "Entity/Scripts/Database/ScriptedEntityList.h"
#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Logging/Logger.h"
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
            player->SendChatMessage(L"§cNot a valid command");
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
            player->SendChatMessage(L"§cYour message include forbidden words and will not be send");
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
    else if (message.substr(0, 6) == L"/kick ")
    {
        std::wstring playerName = message.substr(6, message.size() - 6);
        if (playerName.size() > 0)
        {
            World::WorldManager::Instance().Kick(playerName);
            std::wostringstream confirmMessage;
            confirmMessage << L"§a" << playerName << L" kicked";
            player->SendChatMessage(confirmMessage.str());
        }
    }
    else if (message.substr(0, 5) == L"/ban ")
    {
        std::wstring playerName = message.substr(5, message.size() - 5);
        if (playerName.size() > 0)
        {
            if(World::WorldManager::Instance().Ban(playerName))
            {
                std::wostringstream confirmMessage;
                confirmMessage << L"§a" << playerName << L" banned";
                player->SendChatMessage(confirmMessage.str());
            }
            else
            {
                std::wostringstream confirmMessage;
                confirmMessage << L"§c" << playerName << L" is already banned";
                player->SendChatMessage(confirmMessage.str());
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
                std::wostringstream confirmMessage;
                confirmMessage << L"§a" << playerName << L" unbanned";
                player->SendChatMessage(confirmMessage.str());
            }
            else
            {
                std::wostringstream confirmMessage;
                confirmMessage << L"§c" << playerName << L" is not banned";
                player->SendChatMessage(confirmMessage.str());
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
                std::wostringstream confirmMessage;
                confirmMessage << L"§a" << playerName << L" is now admin";
                player->SendChatMessage(confirmMessage.str());
            }
            else
            {
                std::wostringstream confirmMessage;
                confirmMessage << L"§c" << playerName << L" is already admin";
                player->SendChatMessage(confirmMessage.str());
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
                std::wostringstream confirmMessage;
                confirmMessage << L"§a" << playerName << L" is no longer admin";
                player->SendChatMessage(confirmMessage.str());
            }
            else
            {
                std::wostringstream confirmMessage;
                confirmMessage << L"§c" << playerName << L" is not admin";
                player->SendChatMessage(confirmMessage.str());
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
                std::wostringstream confirmMessage;
                confirmMessage << L"§a" << playerName << L" added to whitelist";
                player->SendChatMessage(confirmMessage.str());
            }
            else
            {
                std::wostringstream confirmMessage;
                confirmMessage << L"§c" << playerName << L" is already in whitelist";
                player->SendChatMessage(confirmMessage.str());
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
                std::wostringstream confirmMessage;
                confirmMessage << L"§a" << playerName << L" is no longer in whitelist";
                player->SendChatMessage(confirmMessage.str());
            }
            else
            {
                std::wostringstream confirmMessage;
                confirmMessage << L"§c" << playerName << L" is not in whitelist";
                player->SendChatMessage(confirmMessage.str());
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
                std::wostringstream confirmMessage;
                confirmMessage << L"§a" << word << L" added to forbidden word list";
                player->SendChatMessage(confirmMessage.str());
            }
            else
            {
                std::wostringstream confirmMessage;
                confirmMessage << L"§c" << word << L" is already in forbidden word list";
                player->SendChatMessage(confirmMessage.str());
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
                std::wostringstream confirmMessage;
                confirmMessage << L"§a" << word << L" is no longer in forbidden word list";
                player->SendChatMessage(confirmMessage.str());
            }
            else
            {
                std::wostringstream confirmMessage;
                confirmMessage << L"§c" << word << L" is not in forbidden word list";
                player->SendChatMessage(confirmMessage.str());
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


} /* namespace Chat */
