#ifndef WINDOW_H_
#define WINDOW_H_

#include "Inventory/ItemStack.h"
#include "Util/types.h"
#include "Window/WindowStaticData.h"

#include <vector>

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
    Window(i_windowId id, const WindowStaticData* windowData);
    virtual ~Window();
    void OpenWindow(World::EntityPlayer* player, int x, i_height y, int z);
    void CloseWindow(World::EntityPlayer* player, bool askByPlayer);
    bool ClickOnWindow(World::EntityPlayer* player, short slotId, char button, short action, char mode, const Inventory::ItemStack* slot);
    void ConfirmTransaction(World::EntityPlayer* player, short action, bool accepted);
    void DoAction(World::EntityPlayer* player, short action);

    void SetSlot(World::EntityPlayer* player, short slotId, const Inventory::ItemStack* slot);
    void SetWindowItems(World::EntityPlayer* player, short slotId, const Inventory::ItemStack* slot);

    void AddInventory(World::EntityPlayer* player, Inventory::Inventory* inventory, int offset);

    i_windowId GetId() const;
    const WindowStaticData* GetWindowData() const;

private:
    const i_windowId id;
    Scripting::WindowScript* script;
    const WindowStaticData* windowData;
    std::vector<Inventory::Inventory*> inventoryList;
};

} /* namespace Window */
#endif /* WINDOW_H_ */
