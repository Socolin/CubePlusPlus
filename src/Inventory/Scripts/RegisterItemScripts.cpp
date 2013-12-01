#include "RegisterItemScripts.h"

#include "Basics/ItemBlockScript.h"
#include "Basics/ItemFrameScript.h"
#include "Basics/ItemPaintingScript.h"
#include "Basics/ItemSignScript.h"
#include "Basics/ItemToolScript.h"

namespace Scripting
{

void RegisterItemScripts()
{
    new ItemBlockScript();
    new ItemFrameScript();
    new ItemPaintingScript();
    new ItemSignScript();
    new ItemToolScript();
}

} /* namespace Scripting */
