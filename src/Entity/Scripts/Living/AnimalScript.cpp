#include "AnimalScript.h"

#include "Network/OpcodeList.h"
#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Util/FloatUtil.h"
#include "World/World.h"

namespace Scripting
{

AnimalScript::AnimalScript()
    : LivingEntityScript("entityliving_animal")
    , nextRandomTick(0)
    , notMoving(false)
    , speed(0.1)
    , frightened(false)
    , frightenedTimer(0)
    , frightenedSpeed(0.2)
    , eggTimer(0)
{
}

AnimalScript::AnimalScript(const std::string& scriptName)
    : LivingEntityScript(scriptName)
    , nextRandomTick(0)
    , notMoving(false)
    , speed(0.1)
    , frightened(false)
    , frightenedTimer(0)
    , frightenedSpeed(0.2)
    , eggTimer(0)
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

    updateRandomDestination(5);

    eggTimer = 6000 + (rand() % 6000);
}

void AnimalScript::OnUpdateTick()
{
    updateEggPop();
    if (frightened)
        updateFrightenedMove();
    else
        updateRandomMove();

}

void AnimalScript::OnReceiveAttack(World::LivingEntity* /*attacker*/, int& /*damage*/)
{
    frightened = true;
    frightenedTimer = 60;
}

void AnimalScript::updateRandomMove()
{
    if (nextRandomTick <= 0)
    {
        updateRandomDestination(5);
    }
    else
        nextRandomTick--;

    if (!notMoving)
    {
        double dx = destination.x - baseEntity->x;
        double dz = destination.z - baseEntity->z;
        float yaw = std::atan2(dz, dx) - M_PI_2;
        baseEntity->Rotate((yaw * 360) / (2 * M_PI), 0);

        double motX = -std::sin((baseEntity->GetYaw() * (2 * M_PI)) / 360) * speed;
        double motZ = std::cos((baseEntity->GetYaw() * (2 * M_PI)) / 360) * speed;
        baseEntity->MoveLiving(motX, motZ);
        if (baseEntity->GetDistance2DSQ(destination) < 0.5)
        {
            updateRandomDestination(5);
        }
        if (((fabs(baseEntity->GetMotionX()) < 0.0001 && fabs(dx) > 0.0001)|| (fabs(baseEntity->GetMotionZ()) == 0.0001 && fabs(dz) > 0.0001)) && baseEntity->isOnGround())
        {
            baseEntity->Jump();
        }
    }
}

void AnimalScript::updateEggPop()
{
    if (eggTimer <= 0)
    {
        eggTimer = 6000 + (rand() % 6000);
        baseEntity->GetWorld()->DropItemstack(*baseEntity, new Inventory::ItemStack(344, 1, 0));
    }
    else
        eggTimer--;
}

void AnimalScript::updateFrightenedMove()
{
    if (frightenedTimer <= 0)
    {
        frightened = false;
    }
    else
        frightenedTimer--;


    if (nextRandomTick <= 0)
    {
        updateRandomDestination(10);
    }
    else
        nextRandomTick--;

    double dx = destination.x - baseEntity->x;
    double dz = destination.z - baseEntity->z;
    float yaw = std::atan2(dz, dx) - M_PI_2;
    baseEntity->Rotate((yaw * 360) / (2 * M_PI), 0);

    double motX = -std::sin((baseEntity->GetYaw() * (2 * M_PI)) / 360) * frightenedSpeed;
    double motZ = std::cos((baseEntity->GetYaw() * (2 * M_PI)) / 360) * frightenedSpeed;
    baseEntity->MoveLiving(motX, motZ);
    if (baseEntity->GetDistance2DSQ(destination) < 0.5)
    {
        updateRandomDestination(5);
    }
    if (((fabs(baseEntity->GetMotionX()) < 0.0001 && fabs(dx) > 0.0001) || (fabs(baseEntity->GetMotionZ()) == 0.0001 && fabs(dz) > 0.0001)) && baseEntity->isOnGround())
    {
        baseEntity->Jump();
    }
}

void AnimalScript::updateRandomDestination(float range)
{
    if (rand() % 3 == 0)
    {
        notMoving = true;
    }
    else
    {
        notMoving = false;
        nextRandomTick = 100 + (rand() % 200);
        destination.Relocate(baseEntity->x + Util::randFloat(-range, range), baseEntity->y, baseEntity->z + Util::randFloat(-range, range));
    }
}

} /* namespace Scripting */
