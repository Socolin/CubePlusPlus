#include "RegisterWindowScript.h"

#include "Basics/WindowChestScript.h"
#include "Basics/WindowCraftingTableScript.h"
#include "Basics/WindowEnderChestScript.h"
#include "Basics/WindowFurnaceScript.h"
#include "Basics/WindowLargeChestScript.h"

namespace Scripting
{

void RegisterWindowScript()
{
    new WindowChestScript();
    new WindowCraftingTableScript();
    new WindowEnderChestScript();
    new WindowFurnaceScript();
    new WindowLargeChestScript();
}

} /* namespace Scripting */

