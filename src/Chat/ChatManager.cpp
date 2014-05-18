#include "ChatManager.h"

#include <iostream>
#include <sstream>

#include "Commands/ChatCommandManager.h"
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
        ChatCommandManager::Instance().HandlePlayerChatCommand(player, message);
        /*

        bool commandHandled = false;
        if (player->isAdmin())
            commandHandled |= handleAdminCommand(player, message);

        if (!commandHandled)
        {
            player->GetChat() << Chat::RED << L"Not a valid command" << std::endl;
        }*/
        return true;
    }
    
    if (player->GetChat().IsMuted())
    {
        player->GetChat() << Chat::RED << L"You have been muted by an admin, your message will not be send" << std::endl;
        LOG_INFO << "Muted player " << player->GetUsername() << " try to send message : " << message << std::endl;
        return true;
    }

    std::set<std::wstring>::iterator it;
    std::wstring messageLow = message;
    boost::algorithm::to_lower(messageLow);
    for(it = forbiddenWords.begin(); it != forbiddenWords.end(); ++it)
    {
        if(messageLow.find(*it) != messageLow.npos)
        {
            player->GetChat() << Chat::RED << L"Your message include forbidden words and will not be send" << std::endl;
            LOG_INFO << "Invalid message send by player " << player->GetUsername() << " : " << message << std::endl;
            return true;
        }
    }

    std::wostringstream formatedMessage;
    formatedMessage << L"<" << player->GetUsername() << L"> " << message;
    message = formatedMessage.str();
    LOG_INFO << message << std::endl;
    return false;
}

bool ChatManager::handleAdminCommand(World::EntityPlayer* player, std::wstring& message)
{
    if (message.substr(0, 9) == L"/addword ")
    {
        std::wstring word = message.substr(9, message.size() - 9);
        if (word.size() > 0)
        {
            if(AddForbiddenWord(word))
            {
                player->GetChat() << Chat::GREEN << L"Word " << Chat::GRAY << word << Chat::GREEN << L" added to forbidden word list" << std::endl;
            }
            else
            {
                player->GetChat() << Chat::RED << L"Word " << Chat::GRAY << word << Chat::RED << L" is already in forbidden word list" << std::endl;
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
                player->GetChat() << Chat::GREEN << L"Word " << Chat::GRAY << word << Chat::GREEN << L" is no longer in forbidden word list" << std::endl;
            }
            else
            {
                player->GetChat() << Chat::RED << L"Word " << Chat::GRAY << word << Chat::RED << L" is not in forbidden word list" << std::endl;
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
    if (fwFileName != "NOFILE")
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
}

bool ChatManager::AddForbiddenWord(const std::wstring& word)
{
    std::wstring forbiddenWord = word;
    boost::algorithm::to_lower(forbiddenWord);
    if (!IsForbiddenWord(forbiddenWord))
    {
        forbiddenWords.insert(forbiddenWord);
        if (fwFileName != "NOFILE")
        {
            std::ofstream forbiddenWordsList;
            std::string stringWord;
            Util::WStringToString(forbiddenWord, stringWord);
            forbiddenWordsList.open(fwFileName.c_str(), std::fstream::out | std::fstream::app);
            forbiddenWordsList << stringWord << std::endl;
            forbiddenWordsList.close();
        }
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
        if (fwFileName != "NOFILE")
        {
            std::ofstream forbiddenWordsListFile;
            std::string stringWord;
            forbiddenWordsListFile.open(fwFileName.c_str(), std::fstream::out | std::fstream::trunc);
            for (auto itrPlr = forbiddenWords.begin(); itrPlr != forbiddenWords.end(); itrPlr++)
            {
                Util::WStringToString(*itrPlr, stringWord);
                forbiddenWordsListFile << stringWord << std::endl;
            }
            forbiddenWordsListFile.close();
        }
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
