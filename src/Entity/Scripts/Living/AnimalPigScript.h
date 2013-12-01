#ifndef ANIMALPIGSCRIPT_H_
#define ANIMALPIGSCRIPT_H_

#include "AnimalScript.h"
#include "AI/AIMakeBaby.h"
#include "AI/AIFindFeeder.h"

namespace Scripting
{

class AnimalPigScript : public AnimalScript, public AIMakeBaby, public AIFindFeeder
{
    typedef AnimalScript parent_type;
public:
    AnimalPigScript();
    virtual ~AnimalPigScript();

    virtual LivingEntityScript* Copy() override;

    virtual void Init() override;
    virtual void OnUpdateTick() override;
    virtual void OnReceiveAttack(World::LivingEntity* attacker, int& damage) override;
    virtual void OnDeath() override;
    virtual ItemUseResult OnInteract(World::EntityPlayer* player) override;
    virtual void OnReachDestination() override;
};

} /* namespace Scripting */
#endif /* ANIMALPIGSCRIPT_H_ */
