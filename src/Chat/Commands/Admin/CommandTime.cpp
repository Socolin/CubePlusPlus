#include "CommandTime.h"

#include "Entity/EntityPlayer.h"
#include "World/World.h"
#include "Util/AssertUtil.h"

namespace Chat
{

CommandTime::CommandTime(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
    : ChatCommand(sender, splitedCommand)
    , syntax(SYNTAX_ERROR)
{
}

bool CommandTime::CheckSyntax()
{
    if (checkSyntaxtWith("s:u"))
    {
        syntax = SYNTAX_SUB_TIME;
    }
    else if (checkSyntaxtWith("s"))
    {
        syntax = SYNTAX_DAYNIGHT;
    }
    return syntax != SYNTAX_ERROR;
}

void CommandTime::ExecuteCommand()
{
    World::World* world = getWorldFromSender();
    if (world != nullptr)
    {
        if (syntax == SYNTAX_SUB_TIME)
        {
            if (splitedCommand[1] == "set")
            {
                long int value = std::atol(splitedCommand[2].c_str());
                world->SetTime(value);
                sender.chatStream << COLOR_SYSTEM << "You switch time to " << COLOR_SYSTEM_PARAM << value << std::endl;
            }
            else
            {
                sender.chatStream << COLOR_KO << "Unknown subcommand: " << COLOR_KO_PARAM << splitedCommand[1] << std::endl;
            }
        }
        else if (syntax == SYNTAX_DAYNIGHT)
        {
            if (splitedCommand[1] == "day")
            {
                world->SetTime(1000);
                sender.chatStream << COLOR_SYSTEM << "You switch time to " << COLOR_SYSTEM_PARAM << "day" << std::endl;
            }
            else if (splitedCommand[1] == "night")
            {
                world->SetTime(20000);
                sender.chatStream << COLOR_SYSTEM << "You switch time to " << COLOR_SYSTEM_PARAM << "night" << std::endl;
            }
            else
            {
                sender.chatStream << COLOR_KO << "Unknown value: " << COLOR_KO_PARAM << splitedCommand[1] << std::endl;
            }
        }
    }
}

} /* namespace Chat */

