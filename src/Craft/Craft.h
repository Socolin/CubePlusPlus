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

#define CRAFT_ANY_DATA -1

struct ItemData
{
    ItemData() : itemId(0), itemData(0) {}
    i_item itemId;
    short itemData;
    bool Equals(const Inventory::ItemStack* item) const
    {
        if (item == nullptr)
            return itemId == 0;
        if (itemId == item->getItemId())
        {
            if (itemData == CRAFT_ANY_DATA)
                return true;
            if (itemData == item->getItemData())
                return true;
        }
        return false;
    }
};

class Craft
{
public:
    Craft(char width, char height, i_item resultId, i_damage resultData, int resultQtt, Scripting::CraftScript* script);
    virtual ~Craft();

    const Inventory::ItemStack* GetResult() const;
    bool Match(Inventory::InventoryCraft* craftInventory) const;
    void SetNextSlot(i_item itemId, i_damage itemData);
    char GetHeight() const;
    char GetWidth() const;

private:
    char width;
    char height;
    Inventory::ItemStack* result;
    Scripting::CraftScript* script;
};

} /* namespace Craft */
#endif /* CRAFT_H_ */
