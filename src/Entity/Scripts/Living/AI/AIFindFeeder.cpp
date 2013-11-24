#include "AIFindFeeder.h"

#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Entity/Scripts/LivingEntityScript.h"
#include "Entity/EntityPlayer.h"
#include "World/World.h"

namespace Scripting
{

AIFindFeeder::AIFindFeeder()
    : findFeederTimer(0)
    , findFeederHasFindFeeder(false)
    , findFeederFoodId(0)
    , baseScript(nullptr)
{
}

AIFindFeeder::~AIFindFeeder()
{
}

void AIFindFeeder::findFeederUpdate(World::ScriptedLivingEntity* baseEntity)
{
    if (findFeederTimer <= 0)
    {
        findFeederTimer = 20;
        findFeederSearchFeeder(baseEntity);
    }
    else
        findFeederTimer--;

    if (findFeederHasFindFeeder)
        baseScript->MoveToDestination(0.2);
}

void AIFindFeeder::findFeederInit(LivingEntityScript* script, i_item foodId)
{
    baseScript = script;
    findFeederFoodId = foodId;
}

void AIFindFeeder::findFeederSearchFeeder(World::ScriptedLivingEntity* baseEntity)
{
    // TODO store feeder entity ID ? and update destination each 5 tick, and reduce this call each 50 tick
    findFeederHasFindFeeder = false;
    World::World* world = baseEntity->GetWorld();
    findFeederSearchList.clear();
    world->GetEntitiesInRangeByEntityType(World::ENTITY_TYPE_PLAYER, 0, *baseEntity, 10, findFeederSearchList);
    for (World::Entity* entity : findFeederSearchList)
    {
        World::EntityPlayer* player = dynamic_cast<World::EntityPlayer*>(entity);
        if (player != nullptr)
        {
            const Inventory::ItemStack* handItem = player->LookItemStackInHand();
            if (handItem != nullptr && handItem->getItemId() == findFeederFoodId)
            {
                if (entity->GetDistanceSQ(*baseEntity) > 2)
                    baseScript->SetDestination(entity->x, entity->y, entity->z);
                findFeederHasFindFeeder = true;
                break;
            }
        }
    }
}

bool AIFindFeeder::findFeederHasTarget()
{
    return findFeederHasFindFeeder;
}

void AIFindFeeder::findFeederResetTarget()
{
    findFeederHasFindFeeder = false;
}

} /* namespace Scripting */
