#ifndef WINDOWCHESTSCRIPT_H_
#define WINDOWCHESTSCRIPT_H_

#include "Window/Script/WindowScript.h"

namespace Scripting
{
/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('window_chest',0);
 */
class WindowChestScript : public WindowScript
{
public:
    WindowChestScript();
    WindowChestScript(std::string scriptName);
    virtual ~WindowChestScript();
    virtual WindowScript* Copy() const;

    virtual void OnOpenWindow(World::EntityPlayer* player) override;
    virtual void OnOpenWindow(World::EntityPlayer* player, Block::TileEntity* tileEntity) override;

    virtual int GetInventoryAndSlotShiftClickTarget(Inventory::eInventoryType clickedInventoryType, i_slot slotId, i_slot& targetSlot, const Inventory::ItemStack* slotItemStack, bool& reverseOrder) override;

};

} /* namespace Scripting */
#endif /* WINDOWCHESTSCRIPT_H_ */
