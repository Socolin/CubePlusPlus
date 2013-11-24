#ifndef WINDOWMAINSCRIPT_H_
#define WINDOWMAINSCRIPT_H_

#include "Window/Scripts/WindowScript.h"

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('window_main',0);
 */
class WindowMainScript : public WindowScript
{
public:
    WindowMainScript();
    virtual ~WindowMainScript();

    virtual WindowScript* Copy() const override;

    virtual void OnCloseWindow(World::EntityPlayer* player) override;

    virtual void OnPostClickOnWindow(World::EntityPlayer* player, short slotId, char button, short action, char mode, const Inventory::ItemStack* slot);

    virtual bool OnClickOnWindow(World::EntityPlayer* player, short slotId, char button, short action, char mode, const Inventory::ItemStack* slot, bool& retValue);
    virtual int GetInventoryAndSlotShiftClickTarget(Inventory::eInventoryType clickedInventoryType, i_slot slotId, i_slot& targetSlot, const Inventory::ItemStack* /*slotItemStack*/, bool& reverseInventoriesOrder, bool& reverseSlotOrder);

};

} /* namespace Scripting */
#endif /* WINDOWMAINSCRIPT_H_ */
