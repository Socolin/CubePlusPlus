#ifndef WINDOWSCRIPT_H_
#define WINDOWSCRIPT_H_

#include "Inventory/InventoryConstants.h"
#include "Window/WindowStaticData.h"

#include <vector>

namespace Block
{
class TileEntity;
}
namespace Inventory
{
class Inventory;
class ItemStack;
}
namespace World
{
class EntityPlayer;
}
namespace Window
{
class Window;
}
namespace Scripting
{
class WindowScript
{
public:
    WindowScript(const char* scriptName);
    virtual ~WindowScript();
    virtual WindowScript* Copy() const = 0;

    virtual void Init(Window::Window* baseWindow);

    virtual void InitParam(int /*paramId*/, int /*param*/) {}
    virtual void InitParam(int /*paramId*/, float /*param*/) {}
    virtual void InitParam(int /*paramId*/, const std::string& /*param*/) {}

    virtual void OnOpenWindow(World::EntityPlayer* player);
    virtual void OnOpenWindow(World::EntityPlayer* player, Block::TileEntity* tileEntity);
    virtual void OnCloseWindow(World::EntityPlayer* player);
    virtual bool OnClickOnWindow(World::EntityPlayer* player, short slotId, char button, short action, char mode, const Inventory::ItemStack* slot, bool& retValue);
    virtual void OnPostClickOnWindow(World::EntityPlayer* player, short slotId, char button, short action, char mode, const Inventory::ItemStack* slot);
    virtual void OnConfirmTransaction(World::EntityPlayer* player, short action, bool accepted);
    virtual void OnDoAction(World::EntityPlayer* player, short action);
    virtual int GetInventoryAndSlotShiftClickTarget(Inventory::eInventoryType clickedInventoryType, i_slot slotId, i_slot& targetSlot, const Inventory::ItemStack* slotItemStack, bool& reverseOrder);
protected:
    Window::Window* baseWindow;
};

} /* namespace Scripting */
#endif /* WINDOWSCRIPT_H_ */
