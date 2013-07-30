#include "AnimalSheepScript.h"

#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Entity/EntityPlayer.h"
#include "Util/FloatUtil.h"
#include "World/World.h"

#include "Logging/Logger.h"

namespace Scripting
{

AnimalSheepScript::AnimalSheepScript()
    : AnimalScript("entityliving_sheep")
	, isSheared(false)
	, fleeceColor(0)
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
    EatGrassInit(this);
    SetFleeceColor(GetRandomFleeceColor());
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
            {
                randomMoveUpdate();
            }
            findFeederUpdate(baseEntity);
        }
        makeBabyUpdate(baseEntity);
        if(isSheared || entityAgeIsBaby())
        {
        	EatGrassUpdate(baseEntity);
        }
    }
}

void AnimalSheepScript::OnDeath()
{
    baseEntity->GetWorld()->DropItemstack(*baseEntity, new Inventory::ItemStack(35, 1, GetFleeceColor()));
}

void AnimalSheepScript::OnInteract(World::EntityPlayer* player)
{
	if (makeBabyCanBeInLove())
    {
		i_slot handSlotId = player->GetHandsInventory()->getHandSlotId();
        if (makeBabyTryFallInLove(player->GetHandsInventory()->LookSlot(handSlotId)))
        {
            if (player->GetGameMode() != World::EntityPlayer::GAMEMODE_CREATVE)
            {
                player->GetHandsInventory()->RemoveSomeItemInSlot(handSlotId, 1);
            }
        }
    }
	if(player->LookItemInHand() != nullptr)
	{
		if(player->LookItemInHand()->getItemId() == 359 && !isSheared && !entityAgeIsBaby())
		{
			SetSheared(true);
			baseEntity->GetWorld()->PlaySound(baseEntity->x, baseEntity->y, baseEntity->z, L"mob.sheep.shear", 1.0f, 1.0f,2);
			baseEntity->GetWorld()->DropItemstack(*baseEntity, new Inventory::ItemStack(35, 1 + rand()%3, GetFleeceColor()));
		}
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

char AnimalSheepScript::GetFleeceColor()
{
	return (baseEntity->GetMetadataManager()->GetCharEntityMetadata(16) & 0x0F);
}

char AnimalSheepScript::GetRandomFleeceColor()
{
	int rand100 = rand()%100;
	int rand500 = rand()%500;
	return (char)rand100 < 5?15:(rand100 < 10?7:(rand100 < 15?8:(rand100 < 18?12:(rand500 == 0?6:0))));
}

void AnimalSheepScript::SetFleeceColor(char fleeceColor){
	char status = baseEntity->GetMetadataManager()->GetCharEntityMetadata(16);
	baseEntity->GetMetadataManager()->SetEntityMetadata(16, char((status & 0xF0) | (fleeceColor & 0x0F)));
}

void AnimalSheepScript::EatGrassBonus()
{
	if(entityAgeIsBaby())
	{
		entityAgeGrow(baseEntity, 60);
		if(entityAgeIsBaby())
			EatGrassStart();
	}
	else{
		SetSheared(false);
	}
}

bool AnimalSheepScript::GetSheared()
{
	return (baseEntity->GetMetadataManager()->GetCharEntityMetadata(16) & 0x10) != 0;
}

void AnimalSheepScript::SetSheared(bool shear)
{
	char status = baseEntity->GetMetadataManager()->GetCharEntityMetadata(16);
	if(shear)
	{
		baseEntity->GetMetadataManager()->SetEntityMetadata(16, char((status | 0x10) & 0x1F));
		isSheared = true;
		EatGrassStart();
	}
	else
	{
		baseEntity->GetMetadataManager()->SetEntityMetadata(16, char((status & 0xEF) & 0x1F));
		isSheared = false;
	}
}

} /* namespace Scripting */
