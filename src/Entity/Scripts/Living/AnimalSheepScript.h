#ifndef ANIMALSHEEPSCRIPT_H_
#define ANIMALSHEEPSCRIPT_H_

#include "AnimalScript.h"
#include "AI/AIMakeBaby.h"
#include "AI/AIFindFeeder.h"
#include "AI/AIEatGrass.h"

namespace
{
class ColorUtil;
}

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
    virtual ItemUseResult OnInteract(World::EntityPlayer* player) override;
    virtual void OnReachDestination() override;
    bool GetSheared();
    void SetSheared(bool isSheared);
    void EatGrassBonus();
    char GetFleeceColor();
    char GetBabyFleeceColor(LivingEntityScript* parent1, LivingEntityScript* parent2);
    void SetFleeceColor(char fleeceColor);
protected:
    char GetRandomFleeceColor();
    //virtual void makeBabyInitBabyScript(LivingEntityScript* babyScript) override;
private:
    bool isSheared;
    char fleeceColor;
};

} /* namespace Scripting */
#endif /* ANIMALSHEEPSCRIPT_H_ */
