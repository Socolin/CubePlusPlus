#include "CommandMute.h"

#include "World/WorldManager.h"
#include "Entity/EntityPlayer.h"

namespace Chat
{

CommandMute::CommandMute(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
    : ChatCommand(sender, splitedCommand)
{
}

bool CommandMute::CheckSyntax()
{
    return checkSyntaxtWith("p");
}

void CommandMute::ExecuteCommand()
{
    std::wstring playerName;
    Util::StringToWString(playerName, splitedCommand[1]);
    World::EntityPlayer* destPlayer = nullptr;
    if (World::WorldManager::Instance().Mute(playerName, destPlayer))
    {
        sender.chatStream << COLOR_OK << "Player " << COLOR_OK_PARAM << playerName << COLOR_OK << " is now muted" << std::endl;
        if (destPlayer != nullptr)
        {
            destPlayer->GetChat() << COLOR_SYSTEM << "You have been muted" << std::endl;
        }
    }
    else
    {
        sender.chatStream << COLOR_KO << "Player " << COLOR_KO_PARAM << playerName << COLOR_KO << " is already muted" << std::endl;
    }
}

} /* namespace Chat */
