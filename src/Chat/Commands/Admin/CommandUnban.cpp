#include "CommandUnban.h"

#include "Util/StringUtil.h"
#include "World/WorldManager.h"

namespace Chat
{

CommandUnban::CommandUnban(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
    : ChatCommand(sender, splitedCommand)
{
}

bool CommandUnban::CheckSyntax()
{
    return checkSyntaxtWith("p");
}

void CommandUnban::ExecuteCommand()
{
    std::wstring playerName;
    Util::StringToWString(playerName, splitedCommand[1]);

    if (World::WorldManager::Instance().UnBan(playerName))
    {
        sender.chatStream << COLOR_OK << L"Player " << COLOR_OK_PARAM << playerName << COLOR_OK << L" unbanned" << std::endl;
    }
    else
    {
        sender.chatStream << COLOR_KO << L"The player " << COLOR_KO_PARAM << playerName << COLOR_KO << L" is not banned" << std::endl;
    }
}

} /* namespace Chat */
