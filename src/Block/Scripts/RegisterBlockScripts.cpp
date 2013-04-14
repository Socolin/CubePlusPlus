#include "RegisterBlockScripts.h"

#include "Basics/BlockButtonScript.h"
#include "Basics/BlockDoorScript.h"
#include "Basics/BlockFallingScript.h"
#include "Basics/BlockJackOLanternScript.h"
#include "Basics/BlockJukeboxScript.h"
#include "Basics/BlockLeverScript.h"
#include "Basics/BlockNoteScript.h"
#include "Basics/BlockWoodScript.h"
#include "Basics/StairScript.h"
#include "Basics/TorchScript.h"

namespace Scripting
{
void RegisterBlockScript()
{
    new BlockButtonScript();
    new BlockDoorScript();
    new BlockFallingScript();
    new BlockJackOLanternScript();
	new BlockJukeboxScript();
    new BlockLeverScript();
    new BlockNoteScript();
    new BlockWoodScript();
    new StairScript();
	new TorchScript();
}

} /* namespace Scripting */
