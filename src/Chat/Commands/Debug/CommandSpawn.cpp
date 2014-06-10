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
    else if (checkSyntaxtWith("i:i"))
    {
        syntax = SYNTAX_ENTITY_ID_COUNT;
    }
    else if (checkSyntaxtWith("s:i"))
    {
        syntax = SYNTAX_ENTITY_STR_COUNT;
    }
    else if (checkSyntaxtWith("s"))
    {
        syntax = SYNTAX_ENTITY_STR;
    }
    
    return syntax != SYNTAX_ERROR;
}

void CommandSpawn::ExecuteCommand()
{
    int entityId = -1;
    if (syntax == SYNTAX_ENTITY_STR || syntax == SYNTAX_ENTITY_STR_COUNT)
    {
        if (splitedCommand[1] == "chicken")
        {
            entityId = 1;
        }
        else if (splitedCommand[1] == "cow")
        {
            entityId = 2;
        }
        else if (splitedCommand[1] == "pig")
        {
            entityId = 3;
        }
        else if (splitedCommand[1] == "sheep")
        {
            entityId = 4;
        }
        else if (splitedCommand[1] == "wolf")
        {
            entityId = 5;
        }
        else if (splitedCommand[1] == "zombie")
        {
            entityId = 6;
        }
        else
        {
            sender.chatStream << COLOR_KO << "Invalid entity name" << std::endl;
            return;
        }
    }
    
    else if (syntax == SYNTAX_ENTITY_ID || syntax == SYNTAX_ENTITY_ID_COUNT)
    {
        entityId = std::atoi(splitedCommand[1].c_str());
        if ((entityId < 0) || (entityId > 7))
        {
            sender.chatStream << COLOR_KO << "Invalid entity id" << std::endl;
            return;
        }
    }
    
    if (syntax == SYNTAX_ENTITY_STR)
    {
        World::ScriptedLivingEntity* entity = World::ScriptedEntityList::Instance().CreateNewEntity(entityId, sender.senderPtr.plr->x, sender.senderPtr.plr->y, sender.senderPtr.plr->z);
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
    else if (syntax == SYNTAX_ENTITY_STR_COUNT || syntax == SYNTAX_ENTITY_ID_COUNT)
    {
        int entityCount = std::atoi(splitedCommand[2].c_str());
        World::ScriptedLivingEntity* entity = nullptr;
        int plrX = sender.senderPtr.plr->x, plrY = sender.senderPtr.plr->y, plrZ = sender.senderPtr.plr->z;
        World::World *plrWorld = sender.senderPtr.plr->GetWorld();
        int randX, randZ;
        plrY = sender.senderPtr.plr->y + 1;
        int i, j = 0;
        bool getValidPos;
        for (i = 0; i < entityCount; i++)
        {
            getValidPos = false;
            while (j <= 1000 || !getValidPos)
            {
                randX = plrX + (rand() % 100) - 50;
                randZ = plrZ + (rand() % 100) - 50;
                if (plrWorld->GetBlockId(randX, plrY, randZ) == 0)
                {
                    if (plrWorld->GetBlockId(randX, plrY + 1, randZ) == 0)
                    {
                        getValidPos = true;
                    }
                }
                j++;
            }
            if (!getValidPos)
            {
                sender.chatStream << COLOR_KO << "Failed to get valid random spawn coord" << std::endl;
                break;
            }
            entity = World::ScriptedEntityList::Instance().CreateNewEntity(entityId, randX, plrY, randZ);
            if (entity != nullptr)
            {
                entity->Rotate(sender.senderPtr.plr->GetYaw(), sender.senderPtr.plr->GetPitch());
                plrWorld->AddEntity(entity);
            }
            else
            {
                sender.chatStream << COLOR_KO << "Invalid entity id" << std::endl;
                return;
            }
        }
        sender.chatStream << COLOR_OK << "Spawned " << COLOR_OK_PARAM << entityCount << COLOR_OK << " entities with id : " << COLOR_OK_PARAM << entityId << std::endl;
    }
}

} /* namespace Chat */

