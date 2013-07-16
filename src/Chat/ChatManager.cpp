#include "ChatManager.h"

#include <iostream>
#include <sstream>

#include "Entity/EntityPlayer.h"
#include "Logging/Logger.h"
#include "World/WorldManager.h"
#include "World/World.h"

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
        World::WorldManager::Instance().Stop();
        isCanceled = true;
    }
    if (message == L"/night")
    {
        World::WorldManager::Instance().GetWorld()->SetTime(20000);
        isCanceled = true;
    }
    if (message == L"/day")
    {
        World::WorldManager::Instance().GetWorld()->SetTime(1000);
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
