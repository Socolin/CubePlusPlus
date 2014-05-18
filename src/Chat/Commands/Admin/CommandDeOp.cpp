#include "CommandDeOp.h"

#include "Entity/EntityPlayer.h"
#include "World/WorldManager.h"

namespace Chat
{

CommandDeOp::CommandDeOp(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
    : ChatCommand(sender, splitedCommand)
{
}

bool CommandDeOp::CheckSyntax()
{
    return checkSyntaxtWith("p");
}

void CommandDeOp::ExecuteCommand()
{
    std::wstring playerName;
    Util::StringToWString(playerName, splitedCommand[1]);
    World::EntityPlayer* destPlayer = nullptr;
    if (World::WorldManager::Instance().UnAdmin(playerName, destPlayer))
    {
        sender.chatStream << COLOR_OK << "Player " << COLOR_OK_PARAM << playerName << COLOR_OK << " is no longer admin" << std::endl;
        if (destPlayer != nullptr)
        {
            destPlayer->GetChat() << COLOR_SYSTEM << "You are no longer admin" << std::endl;
        }
    }
    else
    {
        sender.chatStream << COLOR_KO << "Player " << COLOR_KO_PARAM << playerName << COLOR_KO << " is not admin" << std::endl;
    }
}
} /* namespace Chat */
