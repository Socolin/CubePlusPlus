#ifndef WINDOWENDERCHESTSCRIPT_H_
#define WINDOWENDERCHESTSCRIPT_H_

#include "Window/Scripts/WindowScript.h"

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('window_enderchest',0);
 */
class WindowEnderChestScript : public WindowScript
{
public:
    WindowEnderChestScript();
    virtual ~WindowEnderChestScript();
    virtual WindowScript* Copy() const override;

    virtual void OnOpenWindow(World::EntityPlayer* player) override;
    virtual void OnOpenWindow(World::EntityPlayer* player, Block::TileEntity* tileEntity) override;
    virtual void OnCloseWindow(World::EntityPlayer* player) override;
    virtual int GetInventoryAndSlotShiftClickTarget(Inventory::eInventoryType clickedInventoryType, i_slot slotId, i_slot& targetSlot, const Inventory::ItemStack* slotItemStack, bool& reverseOrder) override;
private:
    Block::TileEntity* enderChestTileEntity;
};

} /* namespace Scripting */
#endif /* WINDOWENDERCHESTSCRIPT_H_ */
