#include "AnimalScript.h"

#include "Network/OpcodeList.h"
#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Util/FloatUtil.h"

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
    updateRandomDestination(5);
}

void AnimalScript::OnUpdateTick()
{
    double dx = destination.x - baseEntity->x;
    double dz = destination.z - baseEntity->z;
    float yaw = std::atan2(dz, dx) - M_PI_2;
    baseEntity->Rotate((yaw * 360) / (2 * M_PI), 0);
    baseEntity->Move(-std::sin((baseEntity->GetYaw() * (2 * M_PI)) / 360) * 0.125, 0, std::cos((baseEntity->GetYaw() * (2 * M_PI)) / 360) * 0.125);
    if (baseEntity->GetDistanceSQ(destination) < 0.5)
    {
        updateRandomDestination(5);
    }
}

void AnimalScript::updateRandomDestination(float range)
{
    destination.Relocate(baseEntity->x + Util::randFloat(-range, range), baseEntity->y, baseEntity->z + Util::randFloat(-range, range));
}

} /* namespace Scripting */
