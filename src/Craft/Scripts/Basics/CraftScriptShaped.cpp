#include "CraftScriptShaped.h"

#include "Craft/Craft.h"
#include "Inventory/InventoryCraft.h"

namespace Scripting
{

CraftScriptShaped::CraftScriptShaped()
    : CraftScript("craft_shaped")
    , currentSlot(0)
{
}

CraftScriptShaped::~CraftScriptShaped()
{
}

CraftScript* CraftScriptShaped::Copy()
{
    return new CraftScriptShaped(*this);
}

void CraftScriptShaped::SetNextSlot(i_item itemId, i_damage itemData)
{
    Craft::ItemData& slot = slots[currentSlot % baseCraft->GetWidth()][currentSlot / baseCraft->GetWidth()];
    slot.itemId = itemId;
    slot.itemData = itemData;
    currentSlot++;
}

bool CraftScriptShaped::Match(Inventory::InventoryCraft* craftInventory) const
{
    bool findCraft = true;
    for (int x = 0; x < baseCraft->GetWidth(); x++)
    {
        for (int y = 0; y < baseCraft->GetHeight(); y++)
        {
            if (!slots[x][y].Equals(craftInventory->LookSlot(x, y)))
            {
                findCraft = false;
                break;
            }
        }
    }
    if (findCraft)
        return true;
    findCraft = true;
    for (int x = 0; x < baseCraft->GetWidth(); x++)
    {
        for (int y = 0; y < baseCraft->GetHeight(); y++)
        {
            if (!slots[baseCraft->GetWidth() - 1 - x][y].Equals(craftInventory->LookSlot(x, y)))
            {
                findCraft = false;
                break;
            }
        }
    }
    return findCraft;
}

} /* namespace Scripting */
