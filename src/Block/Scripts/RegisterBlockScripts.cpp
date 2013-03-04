#include "RegisterBlockScripts.h"

#include "Basics/StairScript.h"
#include "Basics/BlockWoodScript.h"

namespace Scripting
{
void RegisterBlockScript()
{
    new BlockWoodScript();
    new StairScript();
}
}
