#include "CommandUnmute.h"

#include "Entity/EntityPlayer.h"
#include "World/WorldManager.h"

namespace Chat
{

CommandUnmute::CommandUnmute(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
    : ChatCommand(sender, splitedCommand)
{
}

bool CommandUnmute::CheckSyntax()
{
    return checkSyntaxtWith("p");
}

void CommandUnmute::ExecuteCommand()
{
    std::wstring playerName;
    Util::StringToWString(playerName, splitedCommand[1]);
    World::EntityPlayer* destPlayer = nullptr;
    if (World::WorldManager::Instance().UnMute(playerName, destPlayer))
    {
        sender.chatStream << COLOR_OK << "Player " << COLOR_OK_PARAM << playerName << COLOR_OK << " is no longer muted" << std::endl;
        if (destPlayer != nullptr)
        {
            destPlayer->GetChat() << COLOR_SYSTEM << "You are no longer muted" << std::endl;
        }
    }
    else
    {
        sender.chatStream << COLOR_KO << "Player " << COLOR_KO_PARAM << playerName << COLOR_KO << " is not muted" << std::endl;
    }
}

} /* namespace Chat */

