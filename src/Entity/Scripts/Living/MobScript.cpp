#include "MobScript.h"

namespace Scripting
{

MobScript::MobScript()
    : LivingEntityScript("entityliving_mob")
{
}

MobScript::MobScript(const std::string& scriptName)
    : LivingEntityScript(scriptName)
{
}

MobScript::~MobScript()
{
}

LivingEntityScript* MobScript::Copy()
{
    return new MobScript(*this);
}

void MobScript::Init()
{
    randomMoveInit(this, 0.2f);
}

void MobScript::OnUpdateTick()
{
    randomMoveUpdate();
}

void MobScript::OnReachDestination()
{
    randomMoveUpdateDestination();
}

} /* namespace Scripting */
