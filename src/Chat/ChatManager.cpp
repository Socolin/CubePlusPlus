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
    bool isCanceled = false;

    LOG_DEBUG << player->GetUsername() << ":" << message << std::endl;
    if (message == L"/stop")
    {
        player->SendChatMessage(L"§aStopping the server");
        World::WorldManager::Instance().Stop();
        isCanceled = true;
    }
    if (message == L"/night")
    {
        player->SendChatMessage(L"§aYou switch time to night");
        World::WorldManager::Instance().GetWorld()->SetTime(20000);
        isCanceled = true;
    }
    if (message == L"/day")
    {
        player->SendChatMessage(L"§aYou switch time to night day");
        World::WorldManager::Instance().GetWorld()->SetTime(1000);
        isCanceled = true;
    }
    if (message == L"/spawn")
    {
        World::ScriptedLivingEntity* entity = World::ScriptedEntityList::Instance().CreateNewEntity(1, player->x, player->y, player->z);
        player->GetWorld()->AddEntity(entity);
        isCanceled = true;
    }
    if (isCanceled)
        return true;

    std::wostringstream formatedMessage;
    formatedMessage << L"<" << player->GetUsername() << L"> " << message;
    message = formatedMessage.str();
    LOG_INFO << message << std::endl;
    return isCanceled;
}

} /* namespace Chat */
