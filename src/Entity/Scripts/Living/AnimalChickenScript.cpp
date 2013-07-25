#include "AnimalChickenScript.h"

#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Entity/EntityPlayer.h"
#include "Util/FloatUtil.h"
#include "World/World.h"

namespace Scripting
{

AnimalChickenScript::AnimalChickenScript()
    : AnimalScript("entityliving_chicken")
{
}

AnimalChickenScript::~AnimalChickenScript()
{
}

LivingEntityScript* AnimalChickenScript::Copy()
{
    return new AnimalChickenScript(*this);
}

void AnimalChickenScript::Init()
{
    parent_type::Init();
    baseEntity->SetFallingSpeedFactor(0.6);
    baseEntity->SetLivingSound(L"mob.chicken.say");
    baseEntity->SetHurtSound(L"mob.chicken.hurt");
    baseEntity->SetDeathSound(L"mob.chicken.hurt");
    baseEntity->SetLivingSoundInterval(120);
    baseEntity->SetWidthHeight(0.3, 0.7);
    DropItemInit(this, 344, 0, 1, 6000, 12000);
    makeBabyInit(this);

}

void AnimalChickenScript::OnUpdateTick()
{
    if (panicMoveIsPanic())
    {
        panicMoveUpdate();
    }
    else
    {
        if (!makeBabyHasMate())
            randomMoveUpdate();
        DropItemUpdate(baseEntity);
        makeBabyUpdate(baseEntity);
    }
}

void AnimalChickenScript::OnDeath()
{
    baseEntity->GetWorld()->DropItemstack(*baseEntity, new Inventory::ItemStack(365, 1, 0));
}

void AnimalChickenScript::OnInteract(World::EntityPlayer* player)
{
    i_slot handSlotId = player->GetHandsInventory()->getHandSlotId();
    if (makeBabyTryFallInLove(player->GetHandsInventory()->LookSlot(handSlotId)))
    {
        if (player->GetGameMode() != World::EntityPlayer::GAMEMODE_CREATVE)
            player->GetHandsInventory()->RemoveSomeItemInSlot(handSlotId, 1);
    }
}

void AnimalChickenScript::OnReachDestination()
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
