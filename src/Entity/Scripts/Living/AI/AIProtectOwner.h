/*
 * AIProtectOwner.h
 *
 *  Created on: Aug 6, 2013
 *      Author: yann291
 */

#ifndef AIPROTECTOWNER_H_
#define AIPROTECTOWNER_H_

namespace World
{
class LivingEntity;
}
namespace Scripting
{
class LivingEntityScript;
class AIProtectOwner {
public:
    AIProtectOwner();
    virtual ~AIProtectOwner();
protected:
    void protectOwnerInit(LivingEntityScript* defender);
    void protectOwnerStartDefending(World::LivingEntity* attacker);
private:
    World::LivingEntity* ownerAttackerScript;
    LivingEntityScript* ownerDefenderScript;
};

} /* namespace Scripting */

#endif /* AIPROTECTOWNER_H_ */
