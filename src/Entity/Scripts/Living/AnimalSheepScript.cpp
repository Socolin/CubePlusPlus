#include "AnimalSheepScript.h"

#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Entity/EntityPlayer.h"
#include "Util/FloatUtil.h"
#include "World/World.h"

namespace Scripting
{

AnimalSheepScript::AnimalSheepScript()
    : AnimalScript("entityliving_sheep")
	, isSheared(false)
	, woolColor(0)
{
}

AnimalSheepScript::~AnimalSheepScript()
{
}

LivingEntityScript* AnimalSheepScript::Copy()
{
    return new AnimalSheepScript(*this);
}

void AnimalSheepScript::Init()
{
    parent_type::Init();
    baseEntity->SetLivingSound(L"mob.sheep.say");
    baseEntity->SetHurtSound(L"mob.sheep.say");
    baseEntity->SetDeathSound(L"mob.sheep.say");
    baseEntity->SetLivingSoundInterval(120);
    baseEntity->SetWidthHeight(0.6, 1.3);
    makeBabyInit(this);
    findFeederInit(this, 296);
}

void AnimalSheepScript::OnUpdateTick()
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

void AnimalSheepScript::OnDeath()
{
    baseEntity->GetWorld()->DropItemstack(*baseEntity, new Inventory::ItemStack(35, 1, 0));
}

void AnimalSheepScript::OnInteract(World::EntityPlayer* player)
{
	i_slot handSlotId = player->GetHandsInventory()->getHandSlotId();
	if (makeBabyCanBeInLove())
    {
        if (makeBabyTryFallInLove(player->GetHandsInventory()->LookSlot(handSlotId)))
        {
            if (player->GetGameMode() != World::EntityPlayer::GAMEMODE_CREATVE)
                player->GetHandsInventory()->RemoveSomeItemInSlot(handSlotId, 1);
        }
    }
	if(player->LookItemInHand()->getItemId() == 359 && !isSheared){
		SetSheared(false);
	}
	else{
		SetSheared(true);
	}
}

void AnimalSheepScript::OnReceiveAttack(World::LivingEntity* attacker, int& damage)
{
    parent_type::OnReceiveAttack(attacker, damage);
    makeBabyResetInLove();
}

void AnimalSheepScript::OnReachDestination()
{
    if (!makeBabyHasMate())
    {
        if (panicMoveIsPanic())
            panicMoveUpdateDestination();
        else
            randomMoveUpdateDestination();
    }
}

int AnimalSheepScript::getFleeceColor() {
      return (baseEntity->GetMetadataManager()->GetIntEntityMetadata(16) & 15);
}

void AnimalSheepScript::SetSheared(bool sheared){
	woolColor = baseEntity->GetMetadataManager()->GetFloatEntityMetadata(16);
	if(sheared){
		baseEntity->GetMetadataManager()->SetEntityMetadata(16, char(woolColor | 16));
		isSheared = false;
	}
	else{
		baseEntity->GetMetadataManager()->SetEntityMetadata(16, char(woolColor & -17));
		isSheared = true;
	}
}

} /* namespace Scripting */
