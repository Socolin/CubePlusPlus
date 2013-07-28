#ifndef ANIMALSHEEPSCRIPT_H_
#define ANIMALSHEEPSCRIPT_H_

#include "AnimalScript.h"
#include "AI/AIMakeBaby.h"
#include "AI/AIFindFeeder.h"

namespace Scripting
{

class AnimalSheepScript : public AnimalScript, public AIMakeBaby, public AIFindFeeder
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
    void SetSheared(bool isSheared);
    int getFleeceColor();
private:
    bool isSheared;
    char woolColor;
};

} /* namespace Scripting */
#endif /* ANIMALSHEEPSCRIPT_H_ */
