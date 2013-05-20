#ifndef WINDOWCRAFTINGTABLESCRIPT_H_
#define WINDOWCRAFTINGTABLESCRIPT_H_

#include "Window/Scripts/WindowScript.h"

namespace Inventory
{
class InventoryCraft;
}
namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('window_craftingtable',0);
 */
class WindowCraftingTableScript : public WindowScript
{
public:
    WindowCraftingTableScript();
    virtual ~WindowCraftingTableScript();
    virtual WindowScript* Copy() const override;

    virtual void OnOpenWindow(World::EntityPlayer* player) override;
    virtual void OnCloseWindow(World::EntityPlayer* player) override;

    virtual bool OnClickOnWindow(World::EntityPlayer* player, short slotId, char button, short action, char mode, const Inventory::ItemStack* slot, bool& retValue) override;
    virtual void OnPostClickOnWindow(World::EntityPlayer* player, short slotId, char button, short action, char mode, const Inventory::ItemStack* slot);
    virtual int GetInventoryAndSlotShiftClickTarget(Inventory::eInventoryType clickedInventoryType, i_slot slotId, i_slot& targetSlot, const Inventory::ItemStack* slotItemStack, bool& reverseOrder) override;
private:
    Inventory::InventoryCraft* craftInventory;
};

} /* namespace Scripting */
#endif /* WINDOWCRAFTINGTABLESCRIPT_H_ */
