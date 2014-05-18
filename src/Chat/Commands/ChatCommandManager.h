#ifndef CHATCOMMANDMANAGER_H_
#define CHATCOMMANDMANAGER_H_

#include "Util/Singleton.h"

#include <map>
#include <vector>
#include <set>

#include "ChatCommand.h"

namespace World
{
class EntityPlayer;
}

namespace Chat
{
typedef ChatCommand* (*CreateChatCommandPrototype)(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
struct RegisteredCommand
{
    size_t senderMask;
    CreateChatCommandPrototype commandClass;
};
class ChatCommand;
class ChatCommandManager : public Util::Singleton<ChatCommandManager>
{
    friend Util::Singleton<ChatCommandManager>;
    ChatCommandManager();
public:
    virtual ~ChatCommandManager();
    void RegisterChatCommand(const std::string& command, CreateChatCommandPrototype commandClass, size_t senderMask);
    void HandlePlayerChatCommand(World::EntityPlayer* plr, const std::wstring& message);
    void HandleConsoleChatCommand(const std::string& message);
private:
    void HandleChatCommand(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    template<typename CharType>
    bool parseMessage(std::vector<std::string> &outSplitedCommand, const std::basic_string<CharType>& message) const;
private:
    std::map<std::string, RegisteredCommand> chatCommandMapping;
};

} /* namespace Chat */

#include "ChatCommandManager.hxx"

#endif /* CHATCOMMANDMANAGER_H_ */
