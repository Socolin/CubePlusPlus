#include "CraftScriptShapeless.h"

#include "Inventory/InventoryCraft.h"

namespace Scripting
{

CraftScriptShapeless::CraftScriptShapeless()
    : CraftScript("craft_shapeless")
    , currentSlot(0)
{
}

CraftScriptShapeless::~CraftScriptShapeless()
{
}

CraftScript* CraftScriptShapeless::Copy()
{
    return new CraftScriptShapeless(*this);
}

void CraftScriptShapeless::SetNextSlot(i_item itemId, i_damage itemData)
{
    Craft::ItemData& slot = slots[currentSlot];
    slot.itemId = itemId;
    slot.itemData = itemData;
    currentSlot++;
}

bool CraftScriptShapeless::Match(Inventory::InventoryCraft* craftInventory) const
{
    bool findCraft = true;

    char found[CRAFT_MAX_WIDTH * CRAFT_MAX_HEIGHT] = {0};
    for (int x = 0; x < craftInventory->GetCraftWidth(); x++)
    {
        for (int y = 0; y < craftInventory->GetCraftHeight(); y++)
        {
            const Inventory::ItemStack* item = craftInventory->LookSlot(x, y);
            if (item == nullptr)
                continue;

            bool findItem = false;
            for (int i = 0; i < currentSlot; i++)
            {
                if (found[i] == 0 && slots[i].Equals(item))
                {
                    found[i] = 1;
                    findItem = true;
                    break;
                }
            }
            if (!findItem)
            {
                findCraft = false;
            }
        }
    }
    return findCraft;
}

} /* namespace Scripting */
