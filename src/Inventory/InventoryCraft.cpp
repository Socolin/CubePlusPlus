#include "InventoryCraft.h"

#include <vector>
#include "Craft/Craft.h"
#include "Craft/CraftManager.h"

namespace Inventory
{

InventoryCraft::InventoryCraft(int width, int height)
    : Inventory(width * height + 1)
    , width(width)
    , height(height)
    , top(0)
    , left(0)
{
}

InventoryCraft::~InventoryCraft()
{
}

int InventoryCraft::GetResultSlotId()
{
    return 0;
}

const ItemStack* InventoryCraft::LookSlot(int x, int y) const
{
    return slot[1 + (x + left) + height * (y + top)];
}

bool InventoryCraft::CanPlayerPlaceItemAt(i_slot slotId)
{
    return slotId != (width * height);
}

void InventoryCraft::PerformCraftChecking()
{
    int maxX = 0;
    int maxY = 0;
    left = width;
    top = height;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        if (slot[1 + x + y * height] != nullptr)
        {
            left = std::min(x, left);
            top = std::min(y, top);
            maxX = std::max(x, maxX);
            maxY = std::max(y, maxY);
        }
    }
    int sizeX = maxX - left + 1;
    int sizeY = maxY - top + 1;

    Craft::CraftManager& manager = Craft::CraftManager::Instance();
    const std::vector<Craft::Craft*>& craftList = manager.GetCraftList(sizeX, sizeY);
    bool foundMatch = false;
    for (Craft::Craft* craft : craftList)
    {
        if (craft->Match(this))
        {
            ClearAndSetSlot(GetResultSlotId(), craft->GetResult()->Copy());
            foundMatch = true;
            break;
        }
    }
    if (!foundMatch)
    {
        ClearSlot(GetResultSlotId());
    }
}

} /* namespace Inventory */
