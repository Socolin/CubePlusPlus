#include "ChatManager.h"

#include <iostream>
#include <sstream>

#include "Entity/EntityPlayer.h"
#include "World/WorldManager.h"

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

    //std::wcout << player->GetUsername() << ":" << message << std::endl;
    if (message == L"/stop")
    {
        World::WorldManager::GetInstance()->Stop();
        isCanceled = true;
    }

    std::wostringstream formatedMessage;
    formatedMessage << L"<" << player->GetUsername() << L"> " << message;
    message = formatedMessage.str();
    std::wcout << message << std::endl;
    return isCanceled;
}

} /* namespace Chat */
