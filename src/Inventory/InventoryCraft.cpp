#include "InventoryCraft.h"

#include <vector>

#include "Craft/Craft.h"
#include "Craft/CraftManager.h"
#include "Entity/EntityPlayer.h"
#include "Item.h"

namespace Inventory
{

InventoryCraft::InventoryCraft(int width, int height)
    : Inventory(width * height + 1, INVENTORY_TYPE_CRAFT)
    , width(width)
    , height(height)
    , craftWidth(0)
    , craftHeight(0)
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
    return slotId != 0;
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
        {
            if (slot[1 + x + y * width] != nullptr)
            {
                left = std::min(x, left);
                top = std::min(y, top);
                maxX = std::max(x, maxX);
                maxY = std::max(y, maxY);
            }
        }
    }
    craftWidth = maxX - left + 1;
    craftHeight = maxY - top + 1;

    Craft::CraftManager& manager = Craft::CraftManager::Instance();
    const std::vector<Craft::Craft*>& craftList = manager.GetCraftList(craftWidth, craftHeight);
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
    if (!foundMatch && craftWidth > 0 && craftHeight > 0)
    {
        const std::vector<Craft::Craft*>& craftList = manager.GetCraftList(0, 0);
        for (Craft::Craft* craft : craftList)
        {
            if (craft->Match(this))
            {
                ClearAndSetSlot(GetResultSlotId(), craft->GetResult()->Copy());
                foundMatch = true;
                break;
            }
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
        i_stackSize countMaxResult = count;
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

void InventoryCraft::DropInventory(World::EntityPlayer* player)
{
    for (size_t i = 0; i < slot.size(); i++)
    {
        if (i != (size_t)GetResultSlotId())
        {
            if (slot[i] != nullptr)
            {
                player->DropItem(slot[i]);
            }
            slot[i] = nullptr;
        }
    }
}

int InventoryCraft::GetCraftHeight() const
{
    return craftHeight;
}

int InventoryCraft::GetCraftWidth() const
{
    return craftWidth;
}

} /* namespace Inventory */
