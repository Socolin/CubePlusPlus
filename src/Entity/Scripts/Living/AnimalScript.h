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
protected:
    void updateRandomDestination(float range);
    World::Position destination;
};

} /* namespace Scripting */
#endif /* ANIMALSCRIPT_H_ */
