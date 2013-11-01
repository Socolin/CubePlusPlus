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
    // TODO Auto-generated constructor stub
    Config::Config::getConfig().lookupValue("server.general.forbidden-words-file", fwFileName);
    loadForbiddenWordsList();
}

ChatManager::~ChatManager()
{
    // TODO Auto-generated destructor stub
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
        player->SendChatMessage(L"§aReloading VIP and Admin list");
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
            World::WorldManager::Instance().Ban(playerName);
            std::wostringstream confirmMessage;
            confirmMessage << L"§a" << playerName << L" banned";
            player->SendChatMessage(confirmMessage.str());
        }
    }
    else if (message.substr(0, 10) == L"/addadmin ")
    {
        std::wstring playerName = message.substr(10, message.size() - 10);
        if (playerName.size() > 0)
        {
            World::WorldManager::Instance().SetAdmin(playerName);
            std::wostringstream confirmMessage;
            confirmMessage << L"§a" << playerName << L" is now admin";
            player->SendChatMessage(confirmMessage.str());
        }
    }
    else if (message.substr(0, 9) == L"/addword ")
    {
        std::wstring word = message.substr(9, message.size() - 9);
        if (word.size() > 0)
        {
            if(AddForbiddenWord(word))
            {
                player->SendChatMessage(L"§aWord added to blacklist");
            }
            else
            {
                player->SendChatMessage(L"§cWord already added to blacklist");
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
        forbiddenWords.insert(forbiddenWord);
    }
    forbiddenWordsList.close();
}

bool ChatManager::AddForbiddenWord(const std::wstring& word)
{
    auto forbiddenWordsItr = forbiddenWords.find(word);
    if (forbiddenWordsItr != forbiddenWords.end())
    {
        return false;
    }
    else
    {
        std::wstring wordLow = word;
        boost::algorithm::to_lower(wordLow);
        forbiddenWords.insert(wordLow);
        std::ofstream forbiddenWordsList;
        std::string stringWord;
        Util::WStringToString(wordLow, stringWord);
        forbiddenWordsList.open(fwFileName.c_str(), std::fstream::out | std::fstream::app);
        forbiddenWordsList << stringWord << std::endl;
        forbiddenWordsList.close();
        return true;
    }
}


} /* namespace Chat */
