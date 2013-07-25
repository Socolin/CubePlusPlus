#ifndef ANIMALCHICKENSCRIPT_H_
#define ANIMALCHICKENSCRIPT_H_

#include "AnimalScript.h"

namespace Scripting
{

class AnimalChickenScript : public AnimalScript
{
    typedef AnimalScript parent_type;
public:
    AnimalChickenScript();
    virtual ~AnimalChickenScript();

    virtual LivingEntityScript* Copy() override;

    virtual void Init() override;
    virtual void OnUpdateTick() override;
    virtual void OnDeath() override;
};

} /* namespace Scripting */
#endif /* ANIMALCHICKENSCRIPT_H_ */
