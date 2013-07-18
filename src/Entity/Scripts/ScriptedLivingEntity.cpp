#include "ScriptedLivingEntity.h"

#include "Network/OpcodeList.h"
#include "Util/AssertUtil.h"
#include "LivingEntityScript.h"

namespace World
{

ScriptedLivingEntity::ScriptedLivingEntity(eEntityType entityType, int entityTypeFlag, double x, double y, double z,
        Scripting::LivingEntityScript* script,
        char entityClientType)
    : LivingEntity(entityType, entityTypeFlag, x, y, z)
    , script(script)
    , entityClientType(entityClientType)
{
    ASSERT(script != nullptr, "script must not be null !");
}

ScriptedLivingEntity::~ScriptedLivingEntity()
{

}

void ScriptedLivingEntity::UpdateTick()
{
    script->OnUpdateTick();
}

void ScriptedLivingEntity::Interact(EntityPlayer* player)
{
    script->OnInteract(player);
}

void ScriptedLivingEntity::Attack(EntityLiving* attacker, int& damage)
{
    script->OnReceiveAttack(attacker, damage);
}

void ScriptedLivingEntity::GetCreatePacket(Network::NetworkPacket& packet)
{
    packet << (unsigned char)Network::OP_SPAWN_MOB;
    packet << entityId << entityClientType;
    packet << networkX << networkY << networkZ;
    packet << (char)  (yaw * 256.f / 360.f) << (char)  (pitch * 256.f / 360.f) << (char)  (yaw * 256.f / 360.f);
    packet << short(0) << short(0) << short(0);
    metadataManager.Write(packet);
}

void ScriptedLivingEntity::GetSpecificUpdatePacket(Network::NetworkPacket& packet)
{
    script->GetUpdatePacket(packet);
}

char ScriptedLivingEntity::GetEntityClientType() const
{
    return entityClientType;
}

} /* namespace World */
