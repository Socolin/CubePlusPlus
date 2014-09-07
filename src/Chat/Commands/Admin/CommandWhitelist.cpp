#include "CommandWhitelist.h"

#include "Util/StringUtil.h"
#include "World/WorldManager.h"

namespace Chat
{

CommandWhitelist::CommandWhitelist(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
    : ChatCommand(sender, splitedCommand)
    , syntax(SYNTAX_ERROR)
{
}

bool CommandWhitelist::CheckSyntax()
{
    if (checkSyntaxtWith("s"))
    {
        syntax = SYNTAX_SUB;
    }
    else if (checkSyntaxtWith("s:p"))
    {
        syntax = SYNTAX_SUB_PLR;
    }
    return syntax != SYNTAX_ERROR;
}

void CommandWhitelist::ExecuteCommand()
{
    if (syntax == SYNTAX_SUB)
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
        else if (splitedCommand[1] == "show")
        {
            if (World::WorldManager::Instance().UseWhitelist())
            {
                std::set<std::wstring>* whitelist = World::WorldManager::Instance().GetWhitelist();
                if (whitelist != nullptr)
                {
                    for (auto itrPlr = whitelist->begin(); itrPlr != whitelist->end(); itrPlr++)
                    {
                        sender.chatStream << BLUE << " - " << *itrPlr << std::endl;
                    }
                }
                else
                {
                    sender.chatStream << COLOR_KO << "Whitelist is empty" << std::endl;    
                }
            }
            else
            {
                sender.chatStream << COLOR_KO << "Whitelist is not enabled" << std::endl;
            }
        }
        else
        {
            BadSyntaxMessage();
        }
    }
    else if (syntax == SYNTAX_SUB_PLR)
    {
        std::wstring playerName;
        Util::StringToWString(playerName, splitedCommand[2]);
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
            BadSyntaxMessage();
        }
    }
}

} /* namespace Chat */
