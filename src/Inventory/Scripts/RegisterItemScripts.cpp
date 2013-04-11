#include "RegisterItemScripts.h"

#include "Inventory/Scripts/Basics/ItemBlockScript.h"
#include "Inventory/Scripts/Basics/ItemFrameScript.h"
#include "Inventory/Scripts/Basics/ItemPaintingScript.h"

namespace Scripting
{

void RegisterItemScripts()
{
    new ItemBlockScript();
    new ItemFrameScript();
    new ItemPaintingScript();
}

} /* namespace Scripting */
