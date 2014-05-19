#include "CommandSpawn.h"

#include "Entity/EntityPlayer.h"
#include "World/WorldManager.h"
#include "World/World.h"
#include "Entity/Scripts/Database/ScriptedEntityList.h"
#include "Entity/Scripts/ScriptedLivingEntity.h"

namespace Chat
{

CommandSpawn::CommandSpawn(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
    : ChatCommand(sender, splitedCommand)
    , syntax(SYNTAX_ERROR)
{
}

bool CommandSpawn::CheckSyntax()
{
    if (checkSyntaxtWith("i"))
    {
        syntax = SYNTAX_ENTITY_ID;
    }
    else if (checkSyntaxtWith("s"))
    {
        syntax = SYNTAX_ENTITY_STR;
    }
    
    return syntax != SYNTAX_ERROR;
}

void CommandSpawn::ExecuteCommand()
{
    if (syntax == SYNTAX_ENTITY_STR)
    {
        World::ScriptedLivingEntity* entity = nullptr;
        if (splitedCommand[1] == "chicken")
        {
            entity = World::ScriptedEntityList::Instance().CreateNewEntity(1, sender.senderPtr.plr->x, sender.senderPtr.plr->y, sender.senderPtr.plr->z);
        }
        else if (splitedCommand[1] == "cow")
        {
            entity = World::ScriptedEntityList::Instance().CreateNewEntity(2, sender.senderPtr.plr->x, sender.senderPtr.plr->y, sender.senderPtr.plr->z);
        }
        else if (splitedCommand[1] == "pig")
        {
            entity = World::ScriptedEntityList::Instance().CreateNewEntity(3, sender.senderPtr.plr->x, sender.senderPtr.plr->y, sender.senderPtr.plr->z);
        }
        else if (splitedCommand[1] == "sheep")
        {
            entity = World::ScriptedEntityList::Instance().CreateNewEntity(4, sender.senderPtr.plr->x, sender.senderPtr.plr->y, sender.senderPtr.plr->z);
        }
        else if (splitedCommand[1] == "wolf")
        {
            entity = World::ScriptedEntityList::Instance().CreateNewEntity(5, sender.senderPtr.plr->x, sender.senderPtr.plr->y, sender.senderPtr.plr->z);
        }
        else if (splitedCommand[1] == "zombie")
        {
            entity = World::ScriptedEntityList::Instance().CreateNewEntity(6, sender.senderPtr.plr->x, sender.senderPtr.plr->y, sender.senderPtr.plr->z);
        }
        
        if (entity != nullptr)
        {
            entity->Rotate(sender.senderPtr.plr->GetYaw(), sender.senderPtr.plr->GetPitch());
            sender.senderPtr.plr->GetWorld()->AddEntity(entity);
            sender.chatStream << COLOR_OK << "Spawned entity : " << COLOR_OK_PARAM << splitedCommand[1] << std::endl;
        }
        else
        {
            sender.chatStream << COLOR_KO << "Invalid entity name" << std::endl;
        }
    }
    else if (syntax == SYNTAX_ENTITY_ID)
    {
        int entityId = std::atoi(splitedCommand[1].c_str());
        if ((entityId > 0) && (entityId < 7))
        {
            World::ScriptedLivingEntity* entity = World::ScriptedEntityList::Instance().CreateNewEntity(entityId, sender.senderPtr.plr->x, sender.senderPtr.plr->y, sender.senderPtr.plr->z);
            if (entity != nullptr)
            {
                entity->Rotate(sender.senderPtr.plr->GetYaw(), sender.senderPtr.plr->GetPitch());
                sender.senderPtr.plr->GetWorld()->AddEntity(entity);
                sender.chatStream << COLOR_OK << "Spawned entity with id : " << COLOR_OK_PARAM << entityId << std::endl;
            }
            else
            {
                sender.chatStream << COLOR_KO << "Invalid entity id" << std::endl;
            }
        }
        else
        {
            sender.chatStream << COLOR_KO << "Invalid entity id, must be between 1 and 6 " << std::endl;
        }
    }
}

} /* namespace Chat */

