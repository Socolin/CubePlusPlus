#ifndef ANIMALWOLFSCRIPT_H_
#define ANIMALWOLFSCRIPT_H_

#include "AnimalScript.h"
#include "AI/AIMakeBaby.h"
#include "AI/AIProtectOwner.h"
#include "AI/AIFollowOwner.h"
#include "LivingEntityTameable.h"

namespace Scripting
{

class AnimalWolfScript : public AnimalScript, public LivingEntityTameable, public AIMakeBaby, public AIProtectOwner, public AIFollowOwner
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
    bool IsAngry();
    void SetAngry(bool value);
    bool IsBegging();
    void SetBegging(bool value);
    char GetCollarColor();
    void SetCollarColor(char color);
protected:
    //virtual void makeBabyInitBabyScript(LivingEntityScript* babyScript) override;
private:
    void entityInit();
    void updateLivingSound();
    char collarColor;
    bool isAngry;
    bool isBegging;
};

} /* namespace Scripting */
#endif /* ANIMALWOLFSCRIPT_H_ */
