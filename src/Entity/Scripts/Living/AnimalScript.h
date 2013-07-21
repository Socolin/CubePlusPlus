#ifndef ANIMALSCRIPT_H_
#define ANIMALSCRIPT_H_

#include "Entity/Scripts/LivingEntityScript.h"
#include "Entity/Position.h"

namespace Scripting
{

class AnimalScript : public LivingEntityScript
{
public:
    AnimalScript();
    AnimalScript(const std::string& scriptName);
    virtual ~AnimalScript();

    virtual void Init() override;

    virtual LivingEntityScript* Copy() override;

    virtual void OnUpdateTick() override;
    virtual void OnReceiveAttack(World::LivingEntity* attacker, int& damage);

protected:
    void updateRandomMove();
    void updateEggPop();
    void updateFrightenedMove();

    // Random move module
    void updateRandomDestination(float range);
    World::Position destination;
    int nextRandomTick;
    bool notMoving;
    float speed;

    // Frightened effect after being attack
    bool frightened;
    int frightenedTimer;
    float frightenedSpeed;

    // Spawn egg module
    int eggTimer;
};

} /* namespace Scripting */
#endif /* ANIMALSCRIPT_H_ */
