#include "AnimalScript.h"

#include "Network/OpcodeList.h"
#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Util/FloatUtil.h"
#include "World/World.h"

namespace Scripting
{

AnimalScript::AnimalScript()
    : LivingEntityScript("entityliving_animal")
{
}

AnimalScript::AnimalScript(const std::string& scriptName)
    : LivingEntityScript(scriptName)
{
}

AnimalScript::~AnimalScript()
{
}

LivingEntityScript* AnimalScript::Copy()
{
    return new AnimalScript(*this);
}

void AnimalScript::Init()
{
    baseEntity->SetFallingSpeedFactor(0.6);
    baseEntity->SetLivingSound(L"mob.chicken.say");
    baseEntity->SetHurtSound(L"mob.chicken.hurt");
    baseEntity->SetDeathSound(L"mob.chicken.hurt");
    baseEntity->SetLivingSoundInterval(120);
    baseEntity->SetWidthHeight(0.3, 0.7);

    randomMoveInit(this, 0.1f);
    panicMoveInit(this, 0.2f);
    DropItemInit(this, 344, 0, 1, 6000, 12000);
}

void AnimalScript::OnUpdateTick()
{
    DropItemUpdate(baseEntity);
    if (panicMoveIsPanic())
        panicMoveUpdate();
    else
        randomMoveUpdate();
}

void AnimalScript::OnReceiveAttack(World::LivingEntity* /*attacker*/, int& /*damage*/)
{
    panicMoveStart();
}

void AnimalScript::OnDeath()
{
    baseEntity->GetWorld()->DropItemstack(*baseEntity, new Inventory::ItemStack(365, 1, 0));
}

void AnimalScript::OnReachDestination()
{
    if (panicMoveIsPanic())
        panicMoveUpdateDestination();
    else
        randomMoveUpdateDestination();
}



} /* namespace Scripting */
