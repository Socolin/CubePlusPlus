#include "LivingEntityScript.h"

#include "Scripting/ScriptManager.h"
#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Util/FloatUtil.h"

namespace Scripting
{

LivingEntityScript::LivingEntityScript(const std::string scriptName)
    : baseEntity(nullptr)
    , reachDestination(false)
{
    ScriptManager::Instance().RegisterScript(scriptName, this);
}

void LivingEntityScript::Init(World::ScriptedLivingEntity* baseEntity)
{
    this->baseEntity = baseEntity;
    destination.Relocate(baseEntity->x, baseEntity->y, baseEntity->z);
    Init();
}

LivingEntityScript::~LivingEntityScript()
{
}

void LivingEntityScript::OnUpdateTick()
{
}

void LivingEntityScript::OnReceiveAttack(World::LivingEntity* /*attacker*/, int& /*damage*/)
{
}

void LivingEntityScript::OnInteract(World::EntityPlayer* /*player*/)
{
}

void LivingEntityScript::GetCreatePacket(Network::NetworkPacket& /*packet*/)
{
}

void LivingEntityScript::Init()
{
}

void LivingEntityScript::GetUpdatePacket(Network::NetworkPacket& /*packet*/)
{
}

void LivingEntityScript::OnDeath()
{
}

void LivingEntityScript::OnReachDestination()
{
}

void LivingEntityScript::MoveToDestination(float speed)
{
    if (!reachDestination)
    {
        double dx = destination.x - baseEntity->x;
        double dz = destination.z - baseEntity->z;
        float yaw = std::atan2(dz, dx) - M_PI_2;
        baseEntity->Rotate((yaw * 360) / (2 * M_PI), 0);
        if (baseEntity->GetDistance2DSQ(destination) < 0.25)
        {
            reachDestination = true;
            OnReachDestination();
            return;
        }

        double motX = -std::sin((baseEntity->GetYaw() * (2 * M_PI)) / 360) * speed;
        double motZ = std::cos((baseEntity->GetYaw() * (2 * M_PI)) / 360) * speed;
        baseEntity->MoveLiving(motX, motZ);

        if (((fabs(baseEntity->GetMotionX()) < 0.0001 && fabs(dx) > 0.0001)|| (fabs(baseEntity->GetMotionZ()) == 0.0001 && fabs(dz) > 0.0001)) && baseEntity->isOnGround())
        {
            baseEntity->Jump();
        }
    }
}

void LivingEntityScript::GenerateDestination(float range)
{
    destination.Relocate(baseEntity->x + Util::randFloat(-range, range), baseEntity->y, baseEntity->z + Util::randFloat(-range, range));
    reachDestination = false;
}

void LivingEntityScript::SetDestination(double x, double y, double z)
{
    destination.Relocate(x, y, z);
    reachDestination = false;
}

} /* namespace Scripting */
