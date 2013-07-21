#ifndef SCRIPTEDLIVINGENTITY_H_
#define SCRIPTEDLIVINGENTITY_H_

#include "Entity/LivingEntity.h"

namespace Scripting
{
class LivingEntityScript;
}
namespace World
{

class ScriptedLivingEntity: public LivingEntity
{
    typedef LivingEntity parent_type;
public:
    ScriptedLivingEntity(eEntityType entityType, int entityTypeFlag, double x, double y, double z,
            Scripting::LivingEntityScript* script,
            char entityClientType);
    virtual ~ScriptedLivingEntity();

    void UpdateTick() override;

    /**
     * Called when player use right click on entity
     * @param player player who clicked
     */
    virtual void Interact(EntityPlayer* player) override;

    /**
     * Called when the entity is attacked
     * @param attacker
     */
    virtual void Attack(LivingEntity* attacker, int& damage) override;

    virtual void GetCreatePacket(Network::NetworkPacket& packet) override;
    virtual void GetSpecificUpdatePacket(Network::NetworkPacket& packet) override;

    char GetEntityClientType() const;

private:
    Scripting::LivingEntityScript* script;
    unsigned char entityClientType;
};

} /* namespace World */
#endif /* SCRIPTEDLIVINGENTITY_H_ */
