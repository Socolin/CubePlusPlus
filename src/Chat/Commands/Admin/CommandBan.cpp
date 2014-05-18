#include "CommandBan.h"

#include "Chat/ChatStream.h"
#include "World/WorldManager.h"
#include "Util/StringUtil.h"

namespace Chat
{

bool CommandBan::CheckSyntax(const std::vector<std::string>& splitedCommand) const
{
    return checkSyntaxtWith("p", splitedCommand);
}

void CommandBan::ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const
{
    std::wstring playerName;
    Util::StringToWString(playerName, splitedCommand[1]);

    if (World::WorldManager::Instance().Ban(playerName))
    {
        sender.chatStream << COLOR_OK << L"Player " << COLOR_OK_PARAM << playerName << COLOR_OK << L" banned" << std::endl;
    }
    else
    {
        sender.chatStream << COLOR_KO << L"The player " << COLOR_KO_PARAM << playerName << COLOR_KO << L" is already banned" << std::endl;
    }
}

} /* namespace Chat */

