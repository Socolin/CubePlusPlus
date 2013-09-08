#include "AIMakeBaby.h"

#include "Entity/Scripts/LivingEntityScript.h"
#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Inventory/ItemStack.h"
#include "World/World.h"
#include "Entity/Scripts/Database/ScriptedEntityList.h"

namespace Scripting
{

AIMakeBaby::AIMakeBaby()
    : makeBabyTimer(0)
    , makeBabyInLoveTimer(0)
    , makeBabyMate(-1)
    , makeBabyFoodId(0)
    , makeBabySpawnBabyTimer(0)
    , makeBabySearchMateBoundingBox(0, 0,  0)
    , baseScript(nullptr)
{
}

AIMakeBaby::~AIMakeBaby()
{
}

void AIMakeBaby::makeBabyInit(LivingEntityScript* script, i_item foodId)
{
    makeBabyFoodId = foodId;
    baseScript = script;
}

void AIMakeBaby::makeBabyUpdate(World::ScriptedLivingEntity* baseEntity)
{
    if (makeBabySpawnBabyTimer > 0)
    {
        makeBabySpawnBabyTimer--;
        if (makeBabySpawnBabyTimer == 0)
        {
            baseEntity->GetMetadataManager()->SetEntityMetadata(12, int(0));
        }
        return;
    }

    if (makeBabySpawnBabyTimer < 0)
    {
        makeBabySpawnBabyTimer++;
        if (makeBabySpawnBabyTimer == 0)
        {
            baseEntity->GetMetadataManager()->SetEntityMetadata(12, int(0));
        }
        return;
    }

    if (makeBabyInLoveTimer > 0)
    {
        if (makeBabyMate == -1)
        {
            if (makeBabyTimer <= 0)
            {
                makeBabyTimer = 5;
                makeBabyFindMate(baseEntity);
            }
            else
                makeBabyTimer--;
        }
        else
        {
            if (makeBabyTimer <= 0)
            {
                makeBabyTimer = 5;
                makeBabyUpdateMate(baseEntity);
            }
            else
                makeBabyTimer--;
            baseScript->MoveToDestination(0.2);
            makeBabyInLoveTimer--;
            if (makeBabyInLoveTimer <= 0)
            {
                makeBabyMate = -1;
            }
        }
    }
}

bool AIMakeBaby::makeBabyIsInLove()
{
    return makeBabyInLoveTimer > 0;
}

bool AIMakeBaby::makeBabyHasMate()
{
    return makeBabyMate != -1;
}

bool AIMakeBaby::makeBabyTryFallInLove(const Inventory::ItemStack* item)
{
    if (item != nullptr && item->getItemId() == makeBabyFoodId)
    {
        makeBabyInLoveTimer = 600;
        return true;
    }
    return false;
}

void AIMakeBaby::makeBabySetMate(int entityId)
{
    makeBabyMate = entityId;
}

void AIMakeBaby::makeBabyFindMate(World::ScriptedLivingEntity* baseEntity)
{
    World::World* world = baseEntity->GetWorld();
    std::vector<World::Entity*> entityList;
    makeBabySearchMateBoundingBox.SetAndExtend(baseEntity->GetBoundingBox(), 8, 8, 8);
    world->GetEntitiesInAABBByEntityType(World::ENTITY_TYPE_SCRIPTEDLIVING, baseEntity->GetEntityId(), makeBabySearchMateBoundingBox, entityList);

    for (World::Entity* entity : entityList)
    {
        if (entity->IsDead())
            continue;
        World::ScriptedLivingEntity* scriptedEntity = dynamic_cast<World::ScriptedLivingEntity*>(entity);
        if (scriptedEntity && scriptedEntity->GetEntityClientType() == baseEntity->GetEntityClientType())
        {
            LivingEntityScript* script = scriptedEntity->GetScript();
            AIMakeBaby* aiScript = dynamic_cast<AIMakeBaby*>(script);
            if (aiScript != nullptr)
            {
                if (aiScript->makeBabyIsInLove() && !aiScript->makeBabyHasMate())
                {
                    makeBabyMate = entity->GetEntityId();
                    aiScript->makeBabySetMate(baseEntity->GetEntityId());

                    double x = (baseEntity->x + entity->x) / 2.;
                    double y = (baseEntity->y + entity->y) / 2.;
                    double z = (baseEntity->z + entity->z) / 2.;
                    script->SetDestination(x, y, z);
                    baseScript->SetDestination(x, y, z);
                }
            }
        }
    }
}

void AIMakeBaby::makeBabySpawnBaby(World::ScriptedLivingEntity* scriptedEntity)
{
    makeBabySpawnBabyTimer = 6000;
    makeBabyInLoveTimer = 0;
    makeBabyTimer = 0;
    makeBabyMate = -1;
    scriptedEntity->GetMetadataManager()->SetEntityMetadata(12, int(6000));

    int entityTypeId = scriptedEntity->GetServerEntityTypeId();
    World::ScriptedLivingEntity* baby = World::ScriptedEntityList::Instance().CreateNewEntity(entityTypeId, scriptedEntity->x, scriptedEntity->y, scriptedEntity->z);
    baby->Rotate(scriptedEntity->GetYaw(), scriptedEntity->GetPitch());
    scriptedEntity->GetWorld()->AddEntity(baby);
    LivingEntityScript* script = baby->GetScript();
    AIMakeBaby* aiScript = dynamic_cast<AIMakeBaby*>(script);
    if (aiScript != nullptr)
    {
        aiScript->makeBabySetBaby(scriptedEntity);
    }
}

bool AIMakeBaby::makeBabyCanBeInLove()
{
    return makeBabySpawnBabyTimer <= 0;
}

void AIMakeBaby::makeBabySetBaby(World::ScriptedLivingEntity* baseEntity)
{
    makeBabySpawnBabyTimer = -24000;
    baseEntity->GetMetadataManager()->SetEntityMetadata(12, int(-24000));
}

bool AIMakeBaby::makeBabyIsBaby()
{
    return makeBabySpawnBabyTimer < 0;
}

void AIMakeBaby::makeBabyResetInLove()
{
    makeBabyInLoveTimer = 0;
}

void AIMakeBaby::makeBabyUpdateMate(World::ScriptedLivingEntity* baseEntity)
{
    World::World* world = baseEntity->GetWorld();
    World::Entity* entity = world->GetEntityById(makeBabyMate);
    if (entity == nullptr || entity->IsDead())
    {
        makeBabyMate = -1;
        makeBabyFindMate(baseEntity);
    }
    else
    {
        double distanceSq = baseEntity->GetDistanceSQ(*entity);
        World::ScriptedLivingEntity* scriptedEntity = dynamic_cast<World::ScriptedLivingEntity*>(entity);
        LivingEntityScript* script = scriptedEntity->GetScript();
        AIMakeBaby* aiScript = dynamic_cast<AIMakeBaby*>(script);
        if (!aiScript->makeBabyIsInLove())
        {
            makeBabyMate = -1;
            makeBabyFindMate(baseEntity);
            return;
        }
        if (distanceSq < 1)
        {
            if (aiScript != nullptr)
            {
                makeBabySpawnBabyTimer = 6000;
                makeBabyInLoveTimer = 0;
                makeBabyTimer = 0;
                makeBabyMate = -1;
                baseEntity->GetMetadataManager()->SetEntityMetadata(12, int(6000));
                aiScript->makeBabySpawnBaby(scriptedEntity);
            }
        }
        else
        {
            if (scriptedEntity && scriptedEntity->GetEntityClientType() == baseEntity->GetEntityClientType())
            {
                LivingEntityScript* script = scriptedEntity->GetScript();
                double x = (baseEntity->x + entity->x) / 2.;
                double y = (baseEntity->y + entity->y) / 2.;
                double z = (baseEntity->z + entity->z) / 2.;
                script->SetDestination(x, y, z);
                baseScript->SetDestination(x, y, z);
            }
        }
    }
}

} /* namespace Scripting */
