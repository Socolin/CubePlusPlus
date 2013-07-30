#include "AnimalWolfScript.h"

#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Entity/EntityPlayer.h"
#include "Util/FloatUtil.h"
#include "World/World.h"

namespace Scripting
{

AnimalWolfScript::AnimalWolfScript()
    : AnimalScript("entityliving_wolf")
{
}

AnimalWolfScript::~AnimalWolfScript()
{
}

LivingEntityScript* AnimalWolfScript::Copy()
{
    return new AnimalWolfScript(*this);
}

void AnimalWolfScript::Init()
{
    parent_type::Init();
    baseEntity->SetHurtSound(L"mob.wolf.hurt");
    baseEntity->SetDeathSound(L"mob.wolf.death");
    baseEntity->SetLivingSoundInterval(120);
    baseEntity->SetWidthHeight(0.6, 1.8);
    makeBabyInit(this);
}

void AnimalWolfScript::OnUpdateTick()
{
    if (panicMoveIsPanic())
    {
        panicMoveUpdate();
    }
    else
    {
        if (!makeBabyHasMate())
        {
        	randomMoveUpdate();
        }
        makeBabyUpdate(baseEntity);
    }
}

void AnimalWolfScript::OnDeath()
{
}

void AnimalWolfScript::OnInteract(World::EntityPlayer* player)
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

void AnimalWolfScript::OnReceiveAttack(World::LivingEntity* attacker, int& damage)
{
    parent_type::OnReceiveAttack(attacker, damage);
    makeBabyResetInLove();
}

void AnimalWolfScript::OnReachDestination()
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
