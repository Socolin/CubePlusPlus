#include "RegisterWindowScript.h"

#include "Basics/WindowChestScript.h"
#include "Basics/WindowFurnaceScript.h"
#include "Basics/WindowLargeChestScript.h"

namespace Scripting
{

void RegisterWindowScript()
{
    new WindowChestScript();
    new WindowFurnaceScript();
    new WindowLargeChestScript();
}

} /* namespace Scripting */

