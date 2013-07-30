#ifndef ANIMALWOLFSCRIPT_H_
#define ANIMALWOLFSCRIPT_H_

#include "AnimalScript.h"
#include "AI/AIMakeBaby.h"

namespace Scripting
{

class AnimalWolfScript : public AnimalScript, public AIMakeBaby
{
    typedef AnimalScript parent_type;
public:
    AnimalWolfScript();
    virtual ~AnimalWolfScript();

    virtual LivingEntityScript* Copy() override;

    virtual void Init() override;
    virtual void OnUpdateTick() override;
    virtual void OnReceiveAttack(World::LivingEntity* attacker, int& damage) override;
    virtual void OnDeath() override;
    virtual void OnInteract(World::EntityPlayer* player) override;
    virtual void OnReachDestination() override;
};

} /* namespace Scripting */
#endif /* ANIMALWOLFSCRIPT_H_ */
