#ifndef ANIMALSCRIPT_H_
#define ANIMALSCRIPT_H_

#include "Entity/Scripts/LivingEntityScript.h"

namespace Scripting
{

class AnimalScript : public LivingEntityScript
{
public:
    AnimalScript();
    AnimalScript(const std::string& scriptName);

    virtual LivingEntityScript* Copy();
    virtual ~AnimalScript();
};

} /* namespace Scripting */
#endif /* ANIMALSCRIPT_H_ */
