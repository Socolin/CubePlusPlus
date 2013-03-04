#include "RegisterBlockScripts.h"

#include "Basics/StairScript.h"
#include "Basics/BlockLeverScript.h"
#include "Basics/BlockWoodScript.h"

namespace Scripting
{
void RegisterBlockScript()
{
    new BlockWoodScript();
    new StairScript();
    new BlockLeverScript();
}
}
