#include "RegisterItemScripts.h"

#include "Inventory/Scripts/Basics/ItemBlockScript.h"
#include "Inventory/Scripts/Basics/ItemFrameScript.h"

namespace Scripting
{

void RegisterItemScripts()
{
    new ItemBlockScript();
    new ItemFrameScript();
}

} /* namespace Scripting */
