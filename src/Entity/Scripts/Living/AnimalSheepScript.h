#ifndef ANIMALSHEEPSCRIPT_H_
#define ANIMALSHEEPSCRIPT_H_

#include "AnimalScript.h"
#include "AI/AIMakeBaby.h"
#include "AI/AIFindFeeder.h"
#include "AI/AIEatGrass.h"

namespace Scripting
{

class AnimalSheepScript : public AnimalScript, public AIMakeBaby, public AIFindFeeder, public AIEatGrass
{
    typedef AnimalScript parent_type;
public:
    AnimalSheepScript();
    virtual ~AnimalSheepScript();

    virtual LivingEntityScript* Copy() override;

    virtual void Init() override;
    virtual void OnUpdateTick() override;
    virtual void OnReceiveAttack(World::LivingEntity* attacker, int& damage) override;
    virtual void OnDeath() override;
    virtual void OnInteract(World::EntityPlayer* player) override;
    virtual void OnReachDestination() override;
    bool GetSheared();
    void SetSheared(bool isSheared);
    void EatGrassBonus();
    char GetFleeceColor();
    char GetRandomFleeceColor();
    void SetFleeceColor(char fleeceColor);
private:
    bool isSheared;
    char fleeceColor;
};

} /* namespace Scripting */
#endif /* ANIMALSHEEPSCRIPT_H_ */
