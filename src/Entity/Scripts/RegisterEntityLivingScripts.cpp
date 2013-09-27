#include "RegisterEntityLivingScripts.h"

#include "Living/AnimalScript.h"
#include "Living/AnimalChickenScript.h"
#include "Living/AnimalCowScript.h"
#include "Living/AnimalPigScript.h"
#include "Living/AnimalSheepScript.h"
#include "Living/AnimalWolfScript.h"

#include "Living/MobScript.h"
#include "Living/MobZombieScript.h"

namespace Scripting
{

void RegisterEntityLivingScript()
{
    new AnimalScript();
    new AnimalChickenScript();
    new AnimalCowScript();
    new AnimalPigScript();
    new AnimalSheepScript();
    new AnimalWolfScript();
    new MobScript();
    new MobZombieScript();
}

} /* namespace Scripting */
