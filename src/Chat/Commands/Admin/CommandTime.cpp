#include "CommandTime.h"

#include "Entity/EntityPlayer.h"
#include "World/World.h"
#include "Util/AssertUtil.h"

namespace Chat
{

void CommandTime::ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const
{
    World::World* world = getWorldFromSender(sender);
    if (world != nullptr)
    {
        if (checkSyntaxtWith("s:u", splitedCommand))
        {
            if (splitedCommand[1] == "set")
            {
                long int value = std::atol(splitedCommand[2].c_str());
                world->SetTime(value);
            }
            else
            {
                sender.chatStream << COLOR_KO << "Unknown subcommand: " << COLOR_KO_PARAM << splitedCommand[1] << std::endl;
            }
        }
        else if (checkSyntaxtWith("s", splitedCommand))
        {
            if (splitedCommand[1] == "day")
            {
                world->SetTime(1000);
                sender.chatStream << COLOR_SYSTEM << "You switch time to day" << std::endl;
            }
            else if (splitedCommand[1] == "night")
            {
                world->SetTime(20000);
                sender.chatStream << COLOR_SYSTEM << "You switch time to night" << std::endl;
            }
            else
            {
                sender.chatStream << COLOR_KO << "Unknown value: " << COLOR_KO_PARAM << splitedCommand[1] << std::endl;
            }
        }
        else
        {
            BadSyntaxMessage(sender);
        }
    }
}

} /* namespace Chat */

