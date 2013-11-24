#include "RegisterWindowScript.h"

#include "Basics/WindowChestScript.h"
#include "Basics/WindowCraftingTableScript.h"
#include "Basics/WindowEnderChestScript.h"
#include "Basics/WindowFurnaceScript.h"
#include "Basics/WindowLargeChestScript.h"
#include "Basics/WindowMainScript.h"

namespace Scripting
{

void RegisterWindowScript()
{
    new WindowChestScript();
    new WindowCraftingTableScript();
    new WindowEnderChestScript();
    new WindowFurnaceScript();
    new WindowLargeChestScript();
    new WindowMainScript();
}

} /* namespace Scripting */

