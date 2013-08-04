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
    EntityTameableInit(this, baseEntity);
    entityInit();
}

void AnimalWolfScript::entityInit()
{
	baseEntity->GetMetadataManager()->SetEntityMetadata(18, baseEntity->GetHealth());
	baseEntity->GetMetadataManager()->SetEntityMetadata(19, char(0));
	baseEntity->GetMetadataManager()->SetEntityMetadata(20, char(0));
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
	if(player->LookItemInHand() == nullptr){
		SetSitting(!IsSitting());
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

char AnimalWolfScript::GetCollarColor()
{
	return baseEntity->GetMetadataManager()->GetCharEntityMetadata(20) & 0x0F;
}

void AnimalWolfScript::SetCollarColor(char color)
{
	baseEntity->GetMetadataManager()->SetEntityMetadata(20, (color & 0x0F));
}

bool AnimalWolfScript::IsAngry()
{
	return (baseEntity->GetMetadataManager()->GetCharEntityMetadata(16) & 0x02) != 0;
}

void AnimalWolfScript::SetAngry(bool value)
{
	char status = baseEntity->GetMetadataManager()->GetCharEntityMetadata(16);
	if(value)
	{
		baseEntity->GetMetadataManager()->SetEntityMetadata(16, char((status | 0x02) & 0x07));
	}
	else
	{
		baseEntity->GetMetadataManager()->SetEntityMetadata(16, char((status & 0xFD) & 0x07));
	}
}

void AnimalWolfScript::updateLivingSound()
{
	if(IsAngry()){
		baseEntity->SetLivingSound(L"mob.wolf.growl");
	}
	else{
		if(rand()%3 == 0){
			if(IsTamed() && (baseEntity->GetHealth() < 10.0F)){
				baseEntity->SetLivingSound(L"mob.wolf.whine");
			}
			else{
				baseEntity->SetLivingSound(L"mob.wolf.panting");
			}
		}
		else{
			baseEntity->SetLivingSound(L"mob.wolf.bark");
		}
	}
}

} /* namespace Scripting */
