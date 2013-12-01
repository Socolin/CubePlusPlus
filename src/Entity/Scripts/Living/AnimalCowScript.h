#ifndef ANIMALCOWSCRIPT_H_
#define ANIMALCOWSCRIPT_H_

#include "AnimalScript.h"
#include "AI/AIMakeBaby.h"
#include "AI/AIFindFeeder.h"

namespace Scripting
{

class AnimalCowScript : public AnimalScript, public AIMakeBaby, public AIFindFeeder
{
    typedef AnimalScript parent_type;
public:
    AnimalCowScript();
    virtual ~AnimalCowScript();

    virtual LivingEntityScript* Copy() override;

    virtual void Init() override;
    virtual void OnUpdateTick() override;
    virtual void OnReceiveAttack(World::LivingEntity* attacker, int& damage) override;
    virtual void OnDeath() override;
    virtual ItemUseResult OnInteract(World::EntityPlayer* player) override;
    virtual void OnReachDestination() override;
};

} /* namespace Scripting */
#endif /* ANIMALCOWSCRIPT_H_ */
