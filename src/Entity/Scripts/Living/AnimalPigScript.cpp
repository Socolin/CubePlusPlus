#include "AnimalPigScript.h"

#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Entity/EntityPlayer.h"
#include "Util/FloatUtil.h"
#include "World/World.h"

namespace Scripting
{

AnimalPigScript::AnimalPigScript()
    : AnimalScript("entityliving_pig")
{
}

AnimalPigScript::~AnimalPigScript()
{
}

LivingEntityScript* AnimalPigScript::Copy()
{
    return new AnimalPigScript(*this);
}

void AnimalPigScript::Init()
{
    parent_type::Init();
    baseEntity->SetLivingSound(L"mob.pig.say");
    baseEntity->SetHurtSound(L"mob.pig.say");
    baseEntity->SetDeathSound(L"mob.pig.death");
    baseEntity->SetLivingSoundInterval(120);
    baseEntity->SetWidthHeight(0.9, 0.9);
    makeBabyInit(this, 391);
    findFeederInit(this, 391);
}

void AnimalPigScript::OnUpdateTick()
{
    if (panicMoveIsPanic())
    {
        panicMoveUpdate();
    }
    else
    {
        if (!makeBabyHasMate())
        {
            if (!findFeederHasTarget())
                randomMoveUpdate();
            findFeederUpdate(baseEntity);
        }
        makeBabyUpdate(baseEntity);
    }
}

void AnimalPigScript::OnDeath()
{
    baseEntity->GetWorld()->DropItemstack(*baseEntity, new Inventory::ItemStack(319, 1, 0));
}

ItemUseResult AnimalPigScript::OnInteract(World::EntityPlayer* player)
{
    if (makeBabyCanBeInLove())
    {
        i_slot handSlotId = player->GetHandsInventory()->getHandSlotId();
        if (makeBabyTryFallInLove(player->GetHandsInventory()->LookSlot(handSlotId)))
        {
            return ItemUseResult{true, false, 1};
        }
    }
    return ItemUseResult{false, false, 0};
}

void AnimalPigScript::OnReceiveAttack(World::LivingEntity* attacker, int& damage)
{
    parent_type::OnReceiveAttack(attacker, damage);
    makeBabyResetInLove();
}

void AnimalPigScript::OnReachDestination()
{
    if (!makeBabyHasMate())
    {
        if (panicMoveIsPanic())
            panicMoveUpdateDestination();
        else
            randomMoveUpdateDestination();
    }
}

} /* namespace Scripting */
