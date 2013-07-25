#ifndef ANIMALCHICKENSCRIPT_H_
#define ANIMALCHICKENSCRIPT_H_

#include "AnimalScript.h"
#include "AI/AIDropItem.h"
#include "AI/AIMakeBaby.h"

namespace Scripting
{

class AnimalChickenScript : public AnimalScript, public AIDropItem, public AIMakeBaby
{
    typedef AnimalScript parent_type;
public:
    AnimalChickenScript();
    virtual ~AnimalChickenScript();

    virtual LivingEntityScript* Copy() override;

    virtual void Init() override;
    virtual void OnUpdateTick() override;
    virtual void OnDeath() override;

    virtual void OnInteract(World::EntityPlayer* player) override;

    virtual void OnReachDestination() override;
};

} /* namespace Scripting */
#endif /* ANIMALCHICKENSCRIPT_H_ */
