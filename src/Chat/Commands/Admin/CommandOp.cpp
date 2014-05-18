#include "CommandOp.h"

#include "Entity/EntityPlayer.h"
#include "World/WorldManager.h"

namespace Chat
{

bool CommandOp::CheckSyntax(const std::vector<std::string>& splitedCommand) const
{
    return checkSyntaxtWith("p", splitedCommand);
}

void CommandOp::ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const
{
    std::wstring playerName;
    Util::StringToWString(playerName, splitedCommand[1]);
    World::EntityPlayer* destPlayer = nullptr;
    if (World::WorldManager::Instance().SetAdmin(playerName, destPlayer))
    {
        sender.chatStream << COLOR_OK << "Player " << COLOR_OK_PARAM << playerName << COLOR_OK << " is now admin" << std::endl;
        if (destPlayer != nullptr)
        {
            destPlayer->GetChat() << COLOR_SYSTEM << "You are now admin" << std::endl;
        }
    }
    else
    {
        sender.chatStream << COLOR_KO << "Player " << COLOR_KO_PARAM << playerName << COLOR_KO << " is already admin" << std::endl;
    }
}

} /* namespace Chat */
