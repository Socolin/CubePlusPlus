#include "AnimalCowScript.h"

#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Entity/EntityPlayer.h"
#include "Util/FloatUtil.h"
#include "World/World.h"

namespace Scripting
{

AnimalCowScript::AnimalCowScript()
    : AnimalScript("entityliving_cow")
{
}

AnimalCowScript::~AnimalCowScript()
{
}

LivingEntityScript* AnimalCowScript::Copy()
{
    return new AnimalCowScript(*this);
}

void AnimalCowScript::Init()
{
    parent_type::Init();
    baseEntity->SetLivingSound(L"mob.cow.say");
    baseEntity->SetHurtSound(L"mob.cow.hurt");
    baseEntity->SetDeathSound(L"mob.cow.hurt");
    baseEntity->SetLivingSoundInterval(120);
    baseEntity->SetWidthHeight(0.9, 1.3);
    makeBabyInit(this, 296);
    findFeederInit(this, 296);
}

void AnimalCowScript::OnUpdateTick()
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

void AnimalCowScript::OnDeath()
{
    baseEntity->GetWorld()->DropItemstack(*baseEntity, new Inventory::ItemStack(363, 1, 0));
}

void AnimalCowScript::OnInteract(World::EntityPlayer* player)
{
    if (makeBabyCanBeInLove())
    {
        i_slot handSlotId = player->GetHandsInventory()->getHandSlotId();
        if (makeBabyTryFallInLove(player->GetHandsInventory()->LookSlot(handSlotId)))
        {
            if (player->GetGameMode() != World::EntityPlayer::GAMEMODE_CREATVE)
                player->GetHandsInventory()->RemoveSomeItemInSlot(handSlotId, 1);
        }
    }
}

void AnimalCowScript::OnReceiveAttack(World::LivingEntity* attacker, int& damage)
{
    parent_type::OnReceiveAttack(attacker, damage);
    makeBabyResetInLove();
}

void AnimalCowScript::OnReachDestination()
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
