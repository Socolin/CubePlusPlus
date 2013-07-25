#ifndef ANIMALSCRIPT_H_
#define ANIMALSCRIPT_H_

#include "Entity/Scripts/LivingEntityScript.h"
#include "Util/types.h"
#include "AI/AIPanicMove.h"
#include "AI/AIRandomMove.h"

namespace Scripting
{

class AnimalScript : public LivingEntityScript, public AIRandomMove, public AIPanicMove
{
    typedef LivingEntityScript parent_type;
public:
    AnimalScript();
    AnimalScript(const std::string& scriptName);
    virtual ~AnimalScript();

    virtual void Init() override;

    virtual LivingEntityScript* Copy() override;

    virtual void OnUpdateTick() override;
    virtual void OnReceiveAttack(World::LivingEntity* attacker, int& damage) override;
    virtual void OnReachDestination() override;
protected:

    // Loot
    // Vector<Inventory::Loot*> lootList;
};

} /* namespace Scripting */
#endif /* ANIMALSCRIPT_H_ */
