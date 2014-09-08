#include "CommandGamemode.h"

#include "Entity/EntityPlayer.h"
#include "Util/StringUtil.h"
#include "World/WorldManager.h"

namespace Chat
{

CommandGamemode::CommandGamemode(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
    : ChatCommand(sender, splitedCommand)
    , syntax(SYNTAX_ERROR)
{
}

bool CommandGamemode::CheckSyntax()
{
    if (checkSyntaxtWith("p:u"))
    {
        syntax = SYNTAX_PLAYER_GAMEMODE;
    }
    else if (checkSyntaxtWith("u"))
    {
        syntax = SYNTAX_GAMEMODE;
    }
    return syntax != SYNTAX_ERROR;
}

void CommandGamemode::ExecuteCommand()
{
    if (syntax == SYNTAX_GAMEMODE)
    {
        if (sender.type == PLAYER)
        {
            int gamemode = std::atoi(splitedCommand[1].c_str());
            sender.senderPtr.plr->SetGameMode(static_cast<World::EntityPlayer::eGameMode>(gamemode));
        }
        else
        {
            BadSyntaxMessage();
        }
    }
    else if (syntax == SYNTAX_PLAYER_GAMEMODE)
    {
        std::wstring targetPlayerName;
        Util::StringToWString(targetPlayerName, splitedCommand[1]);
        World::EntityPlayer* target = World::WorldManager::Instance().GetPlayerByName(targetPlayerName);
        if (target)
        {
            int gamemode = std::atoi(splitedCommand[2].c_str());
            if (target->SetGameMode(static_cast<World::EntityPlayer::eGameMode>(gamemode)))
            {
                sender.chatStream << COLOR_OK_PARAM << targetPlayerName
                        << COLOR_OK << "'s gamemode changed to "
                        << COLOR_OK_PARAM << gamemode << std::endl;
            }
            else
            {
                sender.chatStream << COLOR_KO << "Invalid gamemode: " << COLOR_KO_PARAM << gamemode << std::endl;
            }
        }
    }
}

void CommandGamemode::BadSyntaxMessage() const
{
    sender.chatStream << COLOR_KO << "Bad syntax, available arguments are :" << std::endl;
    sender.chatStream << COLOR_KO << " - <playername> <gamemode> : " << COLOR_KO_PARAM << "Set gamemode of requested player" << std::endl;
    sender.chatStream << COLOR_KO << " - <gamemode> : " << COLOR_KO_PARAM << "Set your gamemode (0: Peaceful, 1: Easy, 2: Normal, 3: Hard)" << std::endl;
}

} /* namespace Chat */
