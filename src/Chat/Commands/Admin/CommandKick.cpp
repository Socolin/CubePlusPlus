#include "CommandKick.h"

#include "World/WorldManager.h"

namespace Chat
{

CommandKick::CommandKick(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
    : ChatCommand(sender, splitedCommand)
{
}

bool CommandKick::CheckSyntax()
{
    return checkSyntaxtWith("p");
}

void CommandKick::ExecuteCommand()
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

