#include "ItemStack.h"

#include "Item.h"

namespace Inventory
{

ItemStack::ItemStack(int id, int stackSize, int itemData)
        : itemId(id)
        , itemData(itemData)
        , stackSize(stackSize)
{

}

ItemStack::~ItemStack()
{
    // TODO Auto-generated destructor stub
}

ItemStack* ItemStack::Copy() const
{
    return new ItemStack(*this);
}

i_damage ItemStack::getItemData() const
{
    return itemData;
}

void ItemStack::setItemData(i_damage itemData)
{
    this->itemData = itemData;
}

i_item ItemStack::getItemId() const
{
    return itemId;
}

char ItemStack::getStackSize() const
{
    return stackSize;
}

void ItemStack::setStackSize(char stackSize)
{
    this->stackSize = stackSize;
}

const Item* ItemStack::getItem() const
{
    if (itemId < 0 || itemId >= ITEM_COUNT)
        return nullptr;
    return ItemList::Instance().getItem(itemId);
}

int ItemStack::GetMaxStackSize() const
{
    const Item* item = getItem();
    if (item == nullptr)
    {
        return 0;
    }
    return item->getMaxStackSize();
}

bool ItemStack::IsSoftEqual(const ItemStack* otherStack) const
{
    return otherStack->itemId == itemId && otherStack->itemData == itemData;
}
} /* namespace Inventory */
