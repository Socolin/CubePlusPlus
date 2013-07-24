#include "AnimalScript.h"

#include "Network/OpcodeList.h"
#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Util/FloatUtil.h"
#include "World/World.h"

namespace Scripting
{

AnimalScript::AnimalScript()
    : LivingEntityScript("entityliving_animal")
    , eggTimer(0)
    , eggMinTimer(6000)
    , eggMaxTimer(12000)
    , eggItemId(344)
    , eggItemData(0)
    , eggQuantity(1)
{
}

AnimalScript::AnimalScript(const std::string& scriptName)
    : LivingEntityScript(scriptName)
    , eggTimer(0)
    , eggMinTimer(6000)
    , eggMaxTimer(12000)
    , eggItemId(344)
    , eggItemData(0)
    , eggQuantity(1)
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

    if (eggMaxTimer == eggMinTimer)
        eggTimer = eggMinTimer;
    else
        eggTimer = eggMinTimer + (rand() % (eggMaxTimer - eggMinTimer));

    baseEntity->SetLivingSound(L"mob.chicken.say");
    baseEntity->SetHurtSound(L"mob.chicken.hurt");
    baseEntity->SetDeathSound(L"mob.chicken.hurt");
    baseEntity->SetLivingSoundInterval(120);
    baseEntity->SetWidthHeight(0.3, 0.7);

    randomMoveInit(this);
    panicMoveInit(this);
}

void AnimalScript::OnUpdateTick()
{
    updateEggPop();
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

void AnimalScript::updateEggPop()
{
    if (eggTimer <= 0)
    {
        if (eggMaxTimer == eggMinTimer)
            eggTimer = eggMinTimer;
        else
            eggTimer = eggMinTimer + (rand() % (eggMaxTimer - eggMinTimer));

        baseEntity->GetWorld()->DropItemstack(*baseEntity, new Inventory::ItemStack(eggItemId, eggQuantity, eggItemData));
        float soundModifier = ((Util::randFloat() - Util::randFloat()) * 0.2f) + 1.f;
        baseEntity->GetWorld()->PlaySound(baseEntity->x, baseEntity->y, baseEntity->z, L"mob.chicken.plop", 1.0f, soundModifier, 2);
    }
    else
        eggTimer--;
}


} /* namespace Scripting */
