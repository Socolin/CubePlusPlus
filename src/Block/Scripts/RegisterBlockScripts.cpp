#include "RegisterBlockScripts.h"

#include "Basics/BlockDoorScript.h"
#include "Basics/StairScript.h"
#include "Basics/BlockLeverScript.h"
#include "Basics/BlockWoodScript.h"
#include "Basics/TorchScript.h"

namespace Scripting
{
void RegisterBlockScript()
{
    new BlockDoorScript();
    new BlockWoodScript();
    new StairScript();
    new BlockLeverScript();
	new TorchScript();
}
}
