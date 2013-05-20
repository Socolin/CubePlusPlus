#ifndef CRAFT_H_
#define CRAFT_H_

#include <array>

#include "CraftContants.h"
#include "Inventory/ItemStack.h"

namespace Inventory
{
class InventoryCraft;
}
namespace Scripting
{
class CraftScript;
}
namespace Craft
{

#define CRAFT_ANY_DATA i_damage(-1)

class Craft
{
public:
    Craft(char width, char height, i_item resultId, i_damage resultData, int resultQtt);
    virtual ~Craft();

    const Inventory::ItemStack* GetResult() const;
    bool Match(Inventory::InventoryCraft* craftInventory) const;
    void SetNextSlot(i_item itemId, i_damage itemData);
private:
    struct ItemData
    {
        ItemData() : itemId(0), itemData(0) {}
        i_item itemId;
        i_damage itemData;
        bool Equals(const Inventory::ItemStack* item) const
        {
            return itemId == item->getItemId() && (itemData == CRAFT_ANY_DATA || itemData == item->getItemData());
        }
    };
    char width;
    char height;
    int currentSlot;
    ItemData slots[CRAFT_MAX_WIDTH][CRAFT_MAX_HEIGHT];
    Inventory::ItemStack* result;
    Scripting::CraftScript* script;
};

} /* namespace Craft */
#endif /* CRAFT_H_ */
