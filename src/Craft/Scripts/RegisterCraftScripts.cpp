#include "RegisterCraftScripts.h"

#include "Basics/CraftScriptShaped.h"
#include "Basics/CraftScriptShapeless.h"
namespace Scripting
{

void RegisterCraftScript()
{
    new CraftScriptShaped();
    new CraftScriptShapeless();
}

} /* namespace Scripting */
