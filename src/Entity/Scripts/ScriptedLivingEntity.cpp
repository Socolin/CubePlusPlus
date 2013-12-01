#include "ScriptedLivingEntity.h"

#include "Network/OpcodeList.h"
#include "Util/AssertUtil.h"
#include "LivingEntityScript.h"
#include "World/World.h"

namespace World
{

ScriptedLivingEntity::ScriptedLivingEntity(eEntityType entityType, int entityTypeFlag, double x, double y, double z,
        Scripting::LivingEntityScript* script,
        char entityClientType, int serverEntityTypeId)
    : LivingEntity(entityType, entityTypeFlag | ENTITY_TYPEFLAG_SCRIPTEDLIVING, x, y, z)
    , script(script)
    , entityClientType(entityClientType)
    , serverEntityTypeId(serverEntityTypeId)
{
    ASSERT(script != nullptr, "script must not be null !");
}

ScriptedLivingEntity::~ScriptedLivingEntity()
{

}

void ScriptedLivingEntity::UpdateTick()
{
    parent_type::UpdateTick();
    updateCollisionWithLivingEntity();
    script->OnUpdateTick();
}

ItemUseResult ScriptedLivingEntity::Interact(EntityPlayer* player)
{
    return script->OnInteract(player);
}

void ScriptedLivingEntity::Attack(LivingEntity* attacker, int& damage)
{
    parent_type::Attack(attacker, damage);
    if (damage >= 0)
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
    parent_type::GetSpecificUpdatePacket(packet);
    script->GetUpdatePacket(packet);
}

char ScriptedLivingEntity::GetEntityClientType() const
{
    return entityClientType;
}

int ScriptedLivingEntity::GetServerEntityTypeId() const
{
    return serverEntityTypeId;
}

Scripting::LivingEntityScript* ScriptedLivingEntity::GetScript() const
{
    return script;
}

void ScriptedLivingEntity::Kill()
{
    parent_type::Kill();
    script->OnDeath();
}

void ScriptedLivingEntity::updateCollisionWithLivingEntity()
{
    Util::AABB box(0,0,0);
    box.SetAndExtend(boundingBox, 0.4, 0, 0.4);
    std::vector<Entity*> entityList;
    world->GetEntitiesInAABBByEntityFlag(ENTITY_TYPEFLAG_LIVING_ENTITY, entityId, box, entityList);

    for (Entity* entity : entityList)
    {
        double dx = entity->x - x;
        double dz = entity->z - z;
        double max = std::max(std::abs(dx), std::abs(dz));

        if (max > 0.01)
        {
            dx /= max;
            dz /= max;

            if (max > 1)
            {
                double factor = 1 / max;
                dx *= factor;
                dz *= factor;
            }
            dx *= 0.05;
            dz *= 0.05;

            Push(-dx, 0.0, -dz);
            entity->Push(dx, 0.0, dz);
        }
    }
}

} /* namespace World */
