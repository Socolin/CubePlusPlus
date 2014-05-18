#include "CommandWhitelist.h"

#include "Util/StringUtil.h"
#include "World/WorldManager.h"

namespace Chat
{

bool CommandWhitelist::CheckSyntax(const std::vector<std::string>& splitedCommand) const
{
    return checkSyntaxtWith("s", splitedCommand) || checkSyntaxtWith("s:p", splitedCommand);
}

void CommandWhitelist::ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const
{
    if (checkSyntaxtWith("s", splitedCommand))
    {
        if (splitedCommand[1] == "on")
        {
            World::WorldManager::Instance().SetUseWhitelist(true);
            sender.chatStream << COLOR_SYSTEM << "Whitelist enabled" << std::endl;
        }
        else if (splitedCommand[1] == "off")
        {
            World::WorldManager::Instance().SetUseWhitelist(false);
            sender.chatStream << COLOR_SYSTEM << "Whitelist disabled" << std::endl;
        }
        else
        {
            BadSyntaxMessage(sender);
        }
    }
    else if (checkSyntaxtWith("s:p", splitedCommand))
    {
        std::wstring playerName;
        Util::StringToWString(playerName, splitedCommand[1]);
        if (splitedCommand[1] == "add")
        {
            if (World::WorldManager::Instance().AddToWhitelist(playerName))
            {
                sender.chatStream << COLOR_OK << "Player " << COLOR_OK_PARAM << playerName << COLOR_OK << " added to whitelist" << std::endl;
            }
            else
            {
                sender.chatStream << COLOR_KO << "Player " << COLOR_KO_PARAM << playerName << COLOR_KO << " is already in whitelist" << std::endl;
            }
        }
        else if (splitedCommand[1] == "remove")
        {
            if (World::WorldManager::Instance().UnWhitelist(playerName))
            {
                sender.chatStream << COLOR_OK << "Player " << COLOR_OK_PARAM << playerName << COLOR_OK << " is no longer in whitelist" << std::endl;
            }
            else
            {
                sender.chatStream << COLOR_KO << "Player " << COLOR_KO_PARAM << playerName << COLOR_KO << " is not in whitelist" << std::endl;
            }
        }
        else
        {
            BadSyntaxMessage(sender);
        }
    }
}

} /* namespace Chat */
