#ifndef WINDOW_H_
#define WINDOW_H_

#include "Inventory/ItemStack.h"
#include "Util/types.h"
#include "Window/WindowStaticData.h"

#include <set>

namespace Scripting
{
class WindowScript;
}

namespace World
{
class EntityPlayer;
}
namespace Inventory
{
class Inventory;
}
namespace Window
{

class Window
{
public:
    Window(i_windowId id, Scripting::WindowScript* script, const WindowStaticData& windowData);
    virtual ~Window();
    void OpenWindow(World::EntityPlayer* player, int x, i_height y, int z);
    void CloseWindow(World::EntityPlayer* player);
    void ClickOnWindow(World::EntityPlayer* player, short slotId, char button, short action, char mode, const Inventory::ItemStack& slot);
    void SetSlot(World::EntityPlayer* player, short slotId, const Inventory::ItemStack& slot);
    void SetWindowItems(World::EntityPlayer* player, short slotId, const Inventory::ItemStack& slot);
    void ConfirmTransaction(World::EntityPlayer* player, short action, bool accepted);
    void DoAction(World::EntityPlayer* player, short action);
private:
    const i_windowId id;
    Scripting::WindowScript* script;
    const WindowStaticData& windowData;
};

} /* namespace Window */
#endif /* WINDOW_H_ */
