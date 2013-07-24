#ifndef ANIMALSCRIPT_H_
#define ANIMALSCRIPT_H_

#include "Entity/Scripts/LivingEntityScript.h"
#include "Util/types.h"
#include "AI/AIRandomMove.h"
#include "AI/AIPanicMove.h"

namespace Scripting
{

class AnimalScript : public LivingEntityScript, public AIRandomMove, public AIPanicMove
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

    // Spawn egg module
    int eggTimer;
    int eggMinTimer;
    int eggMaxTimer;
    i_item eggItemId;
    i_damage eggItemData;
    i_stackSize eggQuantity;

    // Loot
    // Vector<Inventory::Loot*> lootList;
};

} /* namespace Scripting */
#endif /* ANIMALSCRIPT_H_ */
