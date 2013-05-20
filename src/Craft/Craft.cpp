#include "Craft.h"

#include "Inventory/InventoryCraft.h"

namespace Craft
{

Craft::Craft(char width, char height, i_item resultId, i_damage resultData, int resultQtt)
    : width(width)
    , height(height)
    , currentSlot(0)
    , script(nullptr)
{
    result = new Inventory::ItemStack(resultId, resultQtt, resultData);
}

Craft::~Craft()
{
    delete result;
}

const Inventory::ItemStack* Craft::GetResult() const
{
    if (script != nullptr)
    {
        return result;//TODO
    }
    return result;
}

bool Craft::Match(Inventory::InventoryCraft* craftInventory) const
{
    if (script != nullptr)
    {
        return false;//TODO
    }
    bool findCraft = true;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (!slots[x][y].Equals(craftInventory->LookSlot(x, y)))
            {
                findCraft = false;
            }
        }
    }
    if (findCraft)
        return true;
    findCraft = true;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (!slots[width - 1 - x][y].Equals(craftInventory->LookSlot(x, y)))
            {
                findCraft = false;
            }
        }
    }
    return findCraft;
}

void Craft::SetNextSlot(i_item itemId, i_damage itemData)
{
     ItemData& slot = slots[currentSlot % width][currentSlot / width];
    slot.itemId = itemId;
    slot.itemData = itemData;
    currentSlot++;
}

} /* namespace Craft */
