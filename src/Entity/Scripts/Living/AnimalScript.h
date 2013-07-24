#ifndef ANIMALSCRIPT_H_
#define ANIMALSCRIPT_H_

#include "Entity/Scripts/LivingEntityScript.h"
#include "Util/types.h"
#include "AI/AIDropItem.h"
#include "AI/AIPanicMove.h"
#include "AI/AIRandomMove.h"

namespace Scripting
{

class AnimalScript : public LivingEntityScript, public AIRandomMove, public AIPanicMove, public AIDropItem
{
public:
    AnimalScript();
    AnimalScript(const std::string& scriptName);
    virtual ~AnimalScript();

    virtual void Init() override;

    virtual LivingEntityScript* Copy() override;

    virtual void OnUpdateTick() override;
    virtual void OnReceiveAttack(World::LivingEntity* attacker, int& damage) override;
    virtual void OnReachDestination() override;
    virtual void OnDeath() override;
protected:
    void updateEggPop();


    // Loot
    // Vector<Inventory::Loot*> lootList;
};

} /* namespace Scripting */
#endif /* ANIMALSCRIPT_H_ */
