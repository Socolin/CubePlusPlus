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

void ChatCommandManager::RegisterChatCommand(const std::string& command, ChatCommand* ChatCommand)
{
    chatCommandMapping[command] = ChatCommand;
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
            ChatCommand* command = mappingItr->second;
            if (command->CheckSyntax(splitedCommand))
            {
                command->ExecuteCommand(sender, splitedCommand);
            }
            else
            {
                command->BadSyntaxMessage(sender);
            }
        }
        else
        {
            sender.chatStream << RED << L"Not a valid command" << std::endl;
       }
    }
}

} /* namespace Chat */

