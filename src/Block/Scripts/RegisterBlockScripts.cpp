#include "RegisterBlockScripts.h"

#include "Basics/BlockButtonScript.h"
#include "Basics/BlockCactusScript.h"
#include "Basics/BlockChestScript.h"
#include "Basics/BlockCraftingTableScript.h"
#include "Basics/BlockDoorScript.h"
#include "Basics/BlockEnderChestScript.h"
#include "Basics/BlockFallingScript.h"
#include "Basics/BlockFireScript.h"
#include "Basics/BlockFlowingScript.h"
#include "Basics/BlockFluidScript.h"
#include "Basics/BlockFurnaceScript.h"
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
#include "Basics/BlockStationaryScript.h"
#include "Basics/BlockStationaryLavaScript.h"
#include "Basics/BlockTorchScript.h"
#include "Basics/BlockVineScript.h"
#include "Basics/BlockWoodScript.h"

namespace Scripting
{
void RegisterBlockScript()
{
    new BlockButtonScript();
    new BlockCactusScript();
    new BlockChestScript();
    new BlockCraftingTableScript();
    new BlockDoorScript();
    new BlockEnderChestScript();
    new BlockFallingScript();
    new BlockFireScript();
    new BlockFlowingScript();
    new BlockFluidScript();
    new BlockFurnaceScript();
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
    new BlockStationaryScript();
    new BlockStationaryLavaScript();
    new BlockTorchScript();
    new BlockVineScript();
    new BlockWoodScript();
}

} /* namespace Scripting */
