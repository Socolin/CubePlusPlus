#include "BlockCraftingTableScript.h"

#include "Entity/EntityPlayer.h"
#include "Util/AssertUtil.h"
#include "Window/Window.h"
#include "Window/WindowStaticData.h"
#include "Window/WindowList.h"

namespace Scripting
{

BlockCraftingTableScript::BlockCraftingTableScript()
    : BlockScript("block_craftingtable")
    , windowDataId(0)
{
}

BlockCraftingTableScript::~BlockCraftingTableScript()
{
}

BlockScript* BlockCraftingTableScript::Copy()
{
    return new BlockCraftingTableScript(*this);
}

void BlockCraftingTableScript::InitParam(int paramId, int param)
{
    switch (paramId)
    {
    case SCRIPTINGPARAM_BLOCK_CRAFTINGTABLE_WINDOWID:
        windowDataId = param;
        break;
    default:
        AssertSwitchBadDefault(paramId);
        break;
    }
}

bool BlockCraftingTableScript::OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char /*face*/, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    const Window::WindowStaticData* windowData = Window::WindowList::getWindowData(windowDataId);
    if (windowData != nullptr)
    {
        Window::Window* window = new Window::Window(user->GetNextAndSetCurrentWindowId(), user, windowData);
        window->OpenWindow(x, y, z);
    }
    return true;
}

} /* namespace Scripting */
