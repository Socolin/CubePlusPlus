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
            sender.chatStream << COLOR_OK << "Fly speed updated" << std::endl;
        }
        else if (splitedCommand[1] == "walk")
        {
            plr->GetAbilities().SetWalkSpeed(speed);
            plr->GetAbilities().Update();
            sender.chatStream << COLOR_OK << "Walk speed updated" << std::endl;
        }
        else
        {
            BadSyntaxMessage();
        }
    }
}

void CommandSpeed::BadSyntaxMessage() const
{
    sender.chatStream << COLOR_KO << "Bad syntax, available arguments are :" << std::endl;
    sender.chatStream << COLOR_KO << " - fly <speed> : " << COLOR_KO_PARAM << "Set player fly speed to requested value" << std::endl;
    sender.chatStream << COLOR_KO << " - walk <speed> : " << COLOR_KO_PARAM << "Set player walk speed to requested value" << std::endl;
    sender.chatStream << COLOR_KO << "Speed value must be between -0.5 and 0.5" << std::endl;
}

} /* namespace Chat */
