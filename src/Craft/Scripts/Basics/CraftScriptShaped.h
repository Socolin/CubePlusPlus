#ifndef CRAFTSCRIPTSHAPED_H_
#define CRAFTSCRIPTSHAPED_H_

#include "Craft/Scripts/CraftScript.h"

namespace Scripting
{

class CraftScriptShaped : public CraftScript
{
public:
    CraftScriptShaped();
    virtual ~CraftScriptShaped();

    virtual CraftScript* Copy();

    virtual void SetNextSlot(i_item itemId, i_damage itemData);
    virtual bool Match(Inventory::InventoryCraft* craftInventory) const;

private:
    int currentSlot;
    Craft::ItemData slots[CRAFT_MAX_WIDTH][CRAFT_MAX_HEIGHT];
};

} /* namespace Scripting */
#endif /* CRAFTSCRIPTSHAPED_H_ */
