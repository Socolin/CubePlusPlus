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
    randomMoveInit(this, 0.2f);
    panicMoveInit(this, 0.4f);
    entityAgeInit(this);
}

void AnimalScript::OnUpdateTick()
{
    if (panicMoveIsPanic())
        panicMoveUpdate();
    else
    	if(entityAgeIsBaby())
    		entityAgeUpdate(baseEntity);
        randomMoveUpdate();
}

void AnimalScript::OnReceiveAttack(World::LivingEntity* /*attacker*/, int& /*damage*/)
{
    panicMoveStart();
}

void AnimalScript::OnReachDestination()
{
    if (panicMoveIsPanic())
        panicMoveUpdateDestination();
    else
        randomMoveUpdateDestination();
}



} /* namespace Scripting */
