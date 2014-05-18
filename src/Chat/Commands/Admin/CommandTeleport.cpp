#include "CommandTeleport.h"

#include "World/WorldManager.h"
#include "Entity/EntityPlayer.h"

namespace Chat
{

bool Chat::CommandTeleport::CheckSyntax(const std::vector<std::string>& splitedCommand) const
{
    return checkSyntaxtWith("p", splitedCommand) || checkSyntaxtWith("p:p", splitedCommand) || checkSyntaxtWith("f:f:f:f:f", splitedCommand) || checkSyntaxtWith("f:f:f", splitedCommand);
}

void Chat::CommandTeleport::ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const
{
    if (checkSyntaxtWith("p", splitedCommand))
    {
        if (sender.type == PLAYER)
        {
            std::wstring playerDestinationName;
            Util::WStringToString(playerDestinationName, splitedCommand[1]);

            World::EntityPlayer* destination = World::WorldManager::Instance().GetPlayerByName(playerDestinationName);
            if (destination)
            {
                World::EntityPlayer* player = sender.senderPtr.plr;
                if (destination->GetWorld() == player->GetWorld())
                {
                    player->Teleport(destination->x, destination->y, destination->z, destination->GetYaw(), destination->GetPitch());
                }
                else
                {
                    sender.chatStream << COLOR_KO << "Players are not in same world" << std::endl;
                }
            }
            else
            {
                sender.chatStream << COLOR_KO << "Player not found: " << COLOR_KO_PARAM << playerDestinationName << std::endl;
            }
        }
        else
        {
            BadSyntaxMessage(sender);
        }
    }
    else if (checkSyntaxtWith("p:p", splitedCommand))
    {
        std::wstring playerName1;
        std::wstring playerName2;
        Util::WStringToString(playerName1, splitedCommand[1]);
        Util::WStringToString(playerName2, splitedCommand[2]);

        World::EntityPlayer* player1 = World::WorldManager::Instance().GetPlayerByName(playerName1);
        if (player1)
        {
            World::EntityPlayer* player2 = World::WorldManager::Instance().GetPlayerByName(playerName2);
            if (player2)
            {
                if (player2->GetWorld() == player1->GetWorld())
                {
                    player1->Teleport(player2->x, player2->y, player2->z, player2->GetYaw(), player2->GetPitch());
                    sender.chatStream << COLOR_OK << "Teleporting "
                            << COLOR_OK_PARAM << playerName1
                            << COLOR_OK << " to "
                            << COLOR_OK_PARAM << playerName2
                            << std::endl;

                }
                else
                {
                    sender.chatStream << COLOR_KO << "Players are not in same world" << std::endl;
                }
            }
            else
            {
                sender.chatStream << COLOR_KO << "Player not found: " << COLOR_KO_PARAM << playerName2 << std::endl;
            }
        }
        else
        {
            sender.chatStream << COLOR_KO << "Player not found: " << COLOR_KO_PARAM << playerName1 << std::endl;
        }
    }
    else if (checkSyntaxtWith("f:f:f", splitedCommand))
    {
        if (sender.type == PLAYER)
        {
            double x = atof(splitedCommand[1].c_str());
            double y = atof(splitedCommand[2].c_str());
            double z = atof(splitedCommand[3].c_str());
            World::EntityPlayer* player = sender.senderPtr.plr;
            player->Teleport(x, y, z, player->GetYaw(), player->GetPitch());
        }
        else
        {
            BadSyntaxMessage(sender);
        }
    }
    else if (checkSyntaxtWith("f:f:f:f:f", splitedCommand))
    {
        if (sender.type == PLAYER)
        {
            double x = atof(splitedCommand[1].c_str());
            double y = atof(splitedCommand[2].c_str());
            double z = atof(splitedCommand[3].c_str());
            double yaw = atof(splitedCommand[4].c_str());
            double pitch = atof(splitedCommand[5].c_str());
            World::EntityPlayer* player = sender.senderPtr.plr;
            player->Teleport(x, y, z, static_cast<float>(yaw), static_cast<float>(pitch));
        }
        else
        {
            BadSyntaxMessage(sender);
        }
    }
}

} /* namespace Chat */

