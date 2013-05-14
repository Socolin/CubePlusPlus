#include "ItemStack.h"

#include "Item.h"

namespace Inventory
{

ItemStack::ItemStack(int id, int stackSize, int itemData)
        : itemId(id), stackSize(stackSize), itemData(itemData)
{

}

ItemStack::ItemStack()
        : itemId(-1), stackSize(0), itemData(0)
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

int ItemStack::getItemData() const
{
    return itemData;
}

void ItemStack::setItemData(int itemData)
{
    this->itemData = itemData;
}

int ItemStack::getItemId() const
{
    return itemId;
}

void ItemStack::setItemId(int itemId)
{
    this->itemId = itemId;
}

int ItemStack::getStackSize() const
{
    return stackSize;
}

void ItemStack::setStackSize(int stackSize)
{
    this->stackSize = stackSize;
}

void ItemStack::setItem(int id, int stackSize, int itemData)
{
    this->itemId = id;
    this->itemData = itemData;
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
