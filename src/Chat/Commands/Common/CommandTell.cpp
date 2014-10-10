#include "CommandTell.h"

#include "Entity/EntityPlayer.h"
#include "World/WorldManager.h"

namespace Chat
{

CommandTell::CommandTell(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
    : ChatCommand(sender, splitedCommand)
    , syntax(SYNTAX_ERROR)
{
}

bool CommandTell::CheckSyntax()
{
    if (splitedCommand.size() > 2)
    {
        syntax = SYNTAX_PLAYER_MSG;
    }
    return syntax != SYNTAX_ERROR;
}

void CommandTell::ExecuteCommandPlayer(World::EntityPlayer* plr)
{
    std::wstring destPlayerName;
    Util::StringToWString(destPlayerName, splitedCommand[1]);

    World::EntityPlayer* destPlayer = World::WorldManager::Instance().GetPlayerByName(destPlayerName);
    if (destPlayer)
    {
        if (destPlayer != plr)
        {
            ChatStream& destStream = destPlayer->GetChat();
            sender.chatStream << LIGHT_PURPLE << "<to " << DARK_PURPLE << destPlayerName << LIGHT_PURPLE << "> ";
            destStream << LIGHT_PURPLE << "<from " << DARK_PURPLE << plr->GetUsername() << LIGHT_PURPLE << "> ";
            
            for (size_t i = 3; i < splitedCommand.size() - 1; i++)
            {
                sender.chatStream << splitedCommand[i] << " ";
                destStream << splitedCommand[i] << " ";
            }
            
            sender.chatStream << splitedCommand[splitedCommand.size() - 1] << std::endl;
            destStream << splitedCommand[splitedCommand.size() - 1] << std::endl;
        }
        else
        {
            sender.chatStream << COLOR_KO << "You can't send a message to yourself" << std::endl;
        }
    }
    else
    {
        sender.chatStream << COLOR_KO << "Player not found" << std::endl;
    }
}

bool CommandTell::CanUse()
{
    return (sender.type == PLAYER);
}

void CommandTell::BadSyntaxMessage() const
{
    sender.chatStream << COLOR_KO << "Bad syntax, available arguments are :" << std::endl;
    sender.chatStream << COLOR_KO << " - <player> <msg> : " << COLOR_KO_PARAM << "Say <msg> content to <player>" << std::endl;
}

} /* namespace Chat */


