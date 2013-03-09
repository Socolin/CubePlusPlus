#include "RegisterItemScripts.h"

#include "Inventory/Scripts/Basics/ItemBlockScript.h"
#include "Inventory/Scripts/Basics/ItemUseWithBlockScript.h"

namespace Scripting
{

void RegisterItemScripts()
{
    new ItemBlockScript();
    new ItemUseWithBlockScript();
}

} /* namespace Scripting */
