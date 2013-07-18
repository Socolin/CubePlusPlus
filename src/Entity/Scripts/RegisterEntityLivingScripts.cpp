#include "RegisterEntityLivingScripts.h"

#include "Living/AnimalScript.h"

namespace Scripting
{

void RegisterEntityLivingScript()
{
    new AnimalScript();
}

} /* namespace Scripting */
