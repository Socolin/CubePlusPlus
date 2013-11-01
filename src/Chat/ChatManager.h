
#ifndef CHATMANAGER_H_
#define CHATMANAGER_H_

#include <string>
#include <set>

namespace World
{
class EntityPlayer;
}

namespace Chat
{

class ChatManager
{
public:
    ChatManager();
    virtual ~ChatManager();
    bool HandleChatMessage(World::EntityPlayer* player, std::wstring& message);
private:
    bool handleAdminCommand(World::EntityPlayer* player, std::wstring& message);
    std::set<std::wstring> forbiddenWords;
};

} /* namespace Chat */
#endif /* CHATMANAGER_H_ */
