#include "ChatManager.h"

#include <iostream>
#include <sstream>

#include "Entity/EntityPlayer.h"
#include "Logging/Logger.h"
#include "World/WorldManager.h"
#include "World/World.h"
#include "Entity/Scripts/Database/ScriptedEntityList.h"
#include "Entity/Scripts/ScriptedLivingEntity.h"

namespace Chat
{

ChatManager::ChatManager()
{
    // TODO Auto-generated constructor stub

}

ChatManager::~ChatManager()
{
    // TODO Auto-generated destructor stub
}

bool ChatManager::HandleChatMessage(World::EntityPlayer* player, std::wstring& message)
{
    if (message.substr(0, 1) == L"/")
    {
        if (player->isAdmin())
            handleAdminCommand(player, message);
        return true;
    }

    std::wcout << player->GetUsername() << ":" << message << std::endl;

    std::wostringstream formatedMessage;
    formatedMessage << L"<" << player->GetUsername() << L"> " << message;
    message = formatedMessage.str();
    LOG_INFO << message << std::endl;
    return false;
}

void ChatManager::handleAdminCommand(World::EntityPlayer* player, std::wstring& message)
{
    if (message == L"/stop")
    {
        player->SendChatMessage(L"§aStopping the server");
        World::WorldManager::Instance().Stop();
    }
    else if (message == L"/reload")
    {
        player->SendChatMessage(L"§aReloading the server");
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
    else if (message.substr(0, 5) == L"/kick")
    {
        std::wstring playerName = message.substr(6, message.size() - 6);
        if (playerName.size() > 0)
        {
            World::WorldManager::Instance().Kick(playerName);
        }
    }
    else if (message.substr(0, 4) == L"/ban")
    {
        std::wstring playerName = message.substr(5, message.size() - 5);
        if (playerName.size() > 0)
        {
            World::WorldManager::Instance().Ban(playerName);
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
}

} /* namespace Chat */
