#include "AnimalChickenScript.h"

#include "Entity/Scripts/ScriptedLivingEntity.h"
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
}

void AnimalChickenScript::OnUpdateTick()
{
    parent_type::OnUpdateTick();
    DropItemUpdate(baseEntity);
}

void AnimalChickenScript::OnDeath()
{
    baseEntity->GetWorld()->DropItemstack(*baseEntity, new Inventory::ItemStack(365, 1, 0));
}

} /* namespace Scripting */
