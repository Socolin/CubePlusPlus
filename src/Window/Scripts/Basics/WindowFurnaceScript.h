#ifndef WINDOWFURNACESCRIPT_H_
#define WINDOWFURNACESCRIPT_H_

#include "Window/Scripts/WindowScript.h"

namespace Inventory
{
class InventoryFurnace;
}
namespace Scripting
{
/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('window_furnace',0);
 */
class WindowFurnaceScript : public WindowScript
{
    typedef WindowScript parent_type;
public:
    WindowFurnaceScript();
    virtual ~WindowFurnaceScript();
    virtual WindowScript* Copy() const;

    virtual void OnOpenWindow(World::EntityPlayer* player) override;
    virtual void OnOpenWindow(World::EntityPlayer* player, Block::TileEntity* tileEntity) override;
    virtual int GetInventoryAndSlotShiftClickTarget(Inventory::eInventoryType clickedInventoryType, i_slot slotId, i_slot& targetSlot, const Inventory::ItemStack* slotItemStack, bool& reverseInventoriesOrder, bool& reverseSlotOrder) override;
private:
    Inventory::InventoryFurnace* furnaceInventory;
};

} /* namespace Scripting */
#endif /* WINDOWFURNACESCRIPT_H_ */
