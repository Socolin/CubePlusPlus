#include "ChatCommandManager.h"

#include "Util/StringUtil.h"
#include "Entity/EntityPlayer.h"

namespace Chat
{

ChatCommandManager::ChatCommandManager()
{
}

ChatCommandManager::~ChatCommandManager()
{
}

void ChatCommandManager::RegisterChatCommand(const std::string& command, CreateChatCommandPrototype commandClass, size_t senderNask)
{
    RegisteredCommand registeredCommand;
    registeredCommand.commandClass = commandClass;
    registeredCommand.senderMask = senderNask;
    chatCommandMapping[command] = registeredCommand;
}

void ChatCommandManager::HandlePlayerChatCommand(World::EntityPlayer* plr, const std::wstring& message)
{
    CommandSender sender(PLAYER, plr->GetChat());
    sender.senderPtr.plr = plr;

    std::vector<std::string> splitedCommand;
    parseMessage<wchar_t>(splitedCommand, message);

    HandleChatCommand(sender, splitedCommand);
}

void ChatCommandManager::HandleConsoleChatCommand(const std::string& /*message*/)
{
    /*
    CommandSender sender;
    sender.type = CONSOLE;
//    sender.chatStream = nullptr;//FIXME

    std::vector<std::string> splitedCommand;
    parseMessage<char>(splitedCommand, message);

    HandleChatCommand(sender, splitedCommand);*/
}

void ChatCommandManager::HandleChatCommand(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
{
    if (!splitedCommand.empty())
    {
        const std::string command = splitedCommand[0];
        auto mappingItr = chatCommandMapping.find(command);
        if (mappingItr != chatCommandMapping.end())
        {
            RegisteredCommand registeredCommand = mappingItr->second;
            if ((sender.type & registeredCommand.senderMask) != 0)
            {
                ChatCommand* command = registeredCommand.commandClass(sender, splitedCommand);
                if (command->CheckSyntax())
                {
                    command->ExecuteCommand();
                }
                else
                {
                    command->BadSyntaxMessage();
                }
                delete command;
            }
            else
            {
                sender.chatStream << RED << L"Not a valid command for this sender" << std::endl;
            }
        }
        else
        {
            sender.chatStream << RED << L"Not a valid command" << std::endl;
       }
    }
}

} /* namespace Chat */

