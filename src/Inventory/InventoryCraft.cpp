#include "InventoryCraft.h"

#include <vector>

#include "Craft/Craft.h"
#include "Craft/CraftManager.h"
#include "Item.h"

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

ItemStack* InventoryCraft::GetResultItems(int count)
{
    PerformCraftChecking();
    const ItemStack* lookedResultItem = slot[GetResultSlotId()];
    ItemStack* resultItem = nullptr;
    if (lookedResultItem != nullptr)
    {
        if (count < 0)
        {
            count = lookedResultItem->GetMaxStackSize() / lookedResultItem->getStackSize();
        }
        char countMaxResult = count;
        for (int i = 1; i <= width * height; i++)
        {
            ItemStack* item = slot[i];
            if (item != nullptr)
            {
                countMaxResult = std::min(item->getStackSize(), countMaxResult);
            }
        }

        for (int i = 1; i <= width * height; i++)
        {
            ItemStack* item = slot[i];
            if (item != nullptr)
            {
                item->setStackSize(item->getStackSize() - countMaxResult);
                if (item->getStackSize() <= 0)
                {
                    i_item containerId = item->getItem()->getContainerId();
                    if (containerId > 0)
                    {
                        ClearAndSetSlot(i, new ItemStack(containerId, 1, 0));
                    }
                    else
                    {
                        ClearSlot(i);
                    }
                }
            }
        }
        resultItem = lookedResultItem->Copy();
        resultItem->setStackSize(countMaxResult * lookedResultItem->getStackSize());
    }
    PerformCraftChecking();
    return resultItem;
}

} /* namespace Inventory */
