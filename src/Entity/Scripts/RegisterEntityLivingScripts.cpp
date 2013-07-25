#include "RegisterEntityLivingScripts.h"

#include "Living/AnimalScript.h"
#include "Living/AnimalChickenScript.h"

namespace Scripting
{

void RegisterEntityLivingScript()
{
    new AnimalScript();
    new AnimalChickenScript();
}

} /* namespace Scripting */
