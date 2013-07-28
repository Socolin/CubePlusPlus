#include "RegisterEntityLivingScripts.h"

#include "Living/AnimalScript.h"
#include "Living/AnimalChickenScript.h"
#include "Living/AnimalCowScript.h"
#include "Living/AnimalPigScript.h"
#include "Living/AnimalSheepScript.h"

namespace Scripting
{

void RegisterEntityLivingScript()
{
    new AnimalScript();
    new AnimalChickenScript();
    new AnimalCowScript();
    new AnimalPigScript();
    new AnimalSheepScript();
}

} /* namespace Scripting */
