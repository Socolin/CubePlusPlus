#ifndef CRAFTSCRIPTSHAPELESS_H_
#define CRAFTSCRIPTSHAPELESS_H_

#include "Craft/Scripts/CraftScript.h"

namespace Scripting
{

class CraftScriptShapeless : public CraftScript
{
public:
    CraftScriptShapeless();
    virtual ~CraftScriptShapeless();

    virtual CraftScript* Copy();

    virtual void SetNextSlot(i_item itemId, i_damage itemData);
    virtual bool Match(Inventory::InventoryCraft* craftInventory) const;

private:
    int currentSlot;
    Craft::ItemData slots[CRAFT_MAX_WIDTH * CRAFT_MAX_HEIGHT];
};

} /* namespace Scripting */
#endif /* CRAFTSCRIPTSHAPELESS_H_ */
