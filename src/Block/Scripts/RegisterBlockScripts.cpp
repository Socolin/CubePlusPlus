#include "RegisterBlockScripts.h"

#include "Basics/BlockButtonScript.h"
#include "Basics/BlockCactusScript.h"
#include "Basics/BlockDoorScript.h"
#include "Basics/BlockFallingScript.h"
#include "Basics/BlockGrassScript.h"
#include "Basics/BlockJackOLanternScript.h"
#include "Basics/BlockJukeboxScript.h"
#include "Basics/BlockLeverScript.h"
#include "Basics/BlockNoteScript.h"
#include "Basics/BlockQuartzScript.h"
#include "Basics/BlockRedstonePowered.h"
#include "Basics/BlockRedstoneTorchActiveScript.h"
#include "Basics/BlockRedstoneTorchIdleScript.h"
#include "Basics/BlockRedstoneWireScript.h"
#include "Basics/BlockSignGroundScript.h"
#include "Basics/BlockSignWallScript.h"
#include "Basics/BlockStairScript.h"
#include "Basics/BlockTorchScript.h"
#include "Basics/BlockWoodScript.h"

namespace Scripting
{
void RegisterBlockScript()
{
    new BlockButtonScript();
    new BlockCactusScript();
    new BlockDoorScript();
    new BlockFallingScript();
    new BlockGrassScript();
    new BlockJackOLanternScript();
    new BlockJukeboxScript();
    new BlockLeverScript();
    new BlockNoteScript();
    new BlockQuartzScript();
    new BlockRedstonePowered();
    new BlockRedstoneTorchActiveScript();
    new BlockRedstoneTorchIdleScript();
    new BlockRedstoneWireScript();
    new BlockSignGroundScript();
    new BlockSignWallScript();
    new BlockStairScript();
    new BlockTorchScript();
    new BlockWoodScript();
}

} /* namespace Scripting */
