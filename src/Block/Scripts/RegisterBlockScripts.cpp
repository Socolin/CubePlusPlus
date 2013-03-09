#include "RegisterBlockScripts.h"

#include "Basics/BlockDoorScript.h"
#include "Basics/BlockJackOLanternScript.h"
#include "Basics/BlockNoteScript.h"
#include "Basics/StairScript.h"
#include "Basics/BlockLeverScript.h"
#include "Basics/BlockWoodScript.h"
#include "Basics/TorchScript.h"
#include "Basics/BlockJukeboxScript.h"

namespace Scripting
{
void RegisterBlockScript()
{
    new BlockDoorScript();
    new BlockJackOLanternScript();
    new BlockNoteScript();
    new BlockWoodScript();
    new StairScript();
    new BlockLeverScript();
	new TorchScript();
	new BlockJukeboxScript();
}
}
