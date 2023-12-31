#ifndef LIVINGENTITYSCRIPT_H_
#define LIVINGENTITYSCRIPT_H_

#include <string>

#include "Util/types.h"
#include "Network/NetworkPacket.h"
#include "Entity/Position.h"

namespace World
{
class LivingEntity;
class EntityPlayer;
class ScriptedLivingEntity;
}
namespace Scripting
{

class LivingEntityScript
{
public:
    LivingEntityScript(const std::string scriptName);
    virtual ~LivingEntityScript();

    virtual LivingEntityScript* Copy() = 0;

    virtual void InitParam(int /*paramId*/, int /*param*/) {}
    virtual void InitParam(int /*paramId*/, float /*param*/) {}
    virtual void InitParam(int /*paramId*/, const std::string& /*param*/) {}

    void Init(World::ScriptedLivingEntity* baseEntity);

    virtual void Init();

    /**
     * Called each tick to update entity
     */
    virtual void OnUpdateTick();

    /**
     * Called when receive damage from an other entity, final damage applyed to entity can be change
     * by changing value of damage
     * @param attacker entity that attack
     * @param damage the damage that will be done to entity
     */
    virtual void OnReceiveAttack(World::LivingEntity* attacker, int& damage);

    /**
     * Called when a player do a right click on entity
     * @param player
     */
    virtual ItemUseResult OnInteract(World::EntityPlayer* player);

    virtual void GetCreatePacket(Network::NetworkPacket& packet);

    virtual void GetUpdatePacket(Network::NetworkPacket& packet);

    virtual void OnDeath();

    virtual void OnReachDestination();

    void GenerateDestination(float range);
    void SetDestination(double x, double y, double z);
    void MoveToDestination(float speed);

protected:
    World::ScriptedLivingEntity* baseEntity;

    World::Position destination;
    bool reachDestination;
};

} /* namespace Scripting */
#endif /* LIVINGENTITYSCRIPT_H_ */
