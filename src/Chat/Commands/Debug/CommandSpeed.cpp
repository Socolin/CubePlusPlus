#include "CommandSpeed.h"

#include "Entity/EntityPlayer.h"
#include "Entity/PlayerAbilities.h"

namespace Chat
{

CommandSpeed::CommandSpeed(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
    : ChatCommand(sender, splitedCommand)
{
}

bool CommandSpeed::CheckSyntax()
{
    return checkSyntaxtWith("s:f");
}

void CommandSpeed::ExecuteCommandPlayer(World::EntityPlayer* plr)
{
    float speed = std::atof(splitedCommand[2].c_str());
    if (speed > 0.5f || speed < -0.5f)
    {
        sender.chatStream << COLOR_KO << "Invalid speed value, it should be a float between -0.5 and 0.5" << std::endl;
    }
    else
    {
        if (splitedCommand[1] == "fly")
        {
            plr->GetAbilities().SetFlySpeed(speed);
            plr->GetAbilities().Update();
        }
        else if (splitedCommand[1] == "walk")
        {
            plr->GetAbilities().SetWalkSpeed(speed);
            plr->GetAbilities().Update();
        }
        else
        {
            BadSyntaxMessage();
        }
    }
}

} /* namespace Chat */
