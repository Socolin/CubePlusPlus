#include "CommandBan.h"

#include "Chat/ChatStream.h"
#include "World/WorldManager.h"
#include "Util/StringUtil.h"

namespace Chat
{

CommandBan::CommandBan()
{
}

CommandBan::~CommandBan()
{
}

bool CommandBan::CheckSyntax(const std::vector<std::string>& splitedCommand) const
{
    return checkSyntaxtWith("s",splitedCommand);
}

void CommandBan::ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const
{
    std::wstring playerName;
    Util::StringToWString(playerName, splitedCommand[1]);

    if (World::WorldManager::Instance().Ban(playerName))
    {
        (*sender.chatStream) << Chat::BLUE << L"Player " << Chat::GRAY << playerName << Chat::BLUE << L" banned" << std::endl;
    }
    else
    {
        (*sender.chatStream) << Chat::RED << L"The player " << Chat::GRAY << playerName << Chat::RED  << " is already banned" << std::endl;
    }
}

} /* namespace Chat */

