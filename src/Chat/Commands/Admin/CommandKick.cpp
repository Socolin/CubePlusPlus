#include "CommandKick.h"

#include "World/WorldManager.h"

namespace Chat
{

bool CommandKick::CheckSyntax(const std::vector<std::string>& splitedCommand) const
{
    return checkSyntaxtWith("p", splitedCommand);
}

void CommandKick::ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const
{
    std::wstring playerName;
    Util::StringToWString(playerName, splitedCommand[0]);

    if (World::WorldManager::Instance().Kick(playerName))
    {
        std::wostringstream confirmMessage;
        sender.chatStream << COLOR_OK << "Player " << COLOR_OK_PARAM << playerName << COLOR_OK << " kicked" << std::endl;
    }
    else
    {
        sender.chatStream << COLOR_KO << "Player " << COLOR_KO_PARAM << playerName << COLOR_KO << " not found" << std::endl;
    }
}

} /* namespace Chat */

