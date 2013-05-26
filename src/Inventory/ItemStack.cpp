#include "ItemStack.h"

#include <cppnbt.h>
#include "Item.h"

namespace Inventory
{

ItemStack::ItemStack(int id, int stackSize, int itemData)
        : itemId(id)
        , itemData(itemData)
        , stackSize(stackSize)
        , specialData(nullptr)
{

}

ItemStack::ItemStack(const ItemStack& itemStack)
    : itemId(itemStack.itemId)
    , itemData(itemStack.itemData)
    , stackSize(itemStack.stackSize)
    , specialData(nullptr)
{
    if (itemStack.specialData != nullptr)
    {
        specialData = dynamic_cast<nbt::TagCompound*>(itemStack.specialData->clone());
    }
}

ItemStack::ItemStack(nbt::TagCompound* nbtItemData)
    : itemId(0)
    , itemData(0)
    , stackSize(0)
    , specialData(nullptr)
{
    if (nbtItemData)
    {
        nbt::TagShort* tagItemId = nbtItemData->getValueAt<nbt::TagShort>("id");
        if (tagItemId)
        {
            itemId = tagItemId->getValue();
        }
        nbt::TagShort* tagItemDamage = nbtItemData->getValueAt<nbt::TagShort>("Damage");
        if (tagItemDamage)
        {
            itemData = tagItemDamage->getValue();
        }
        nbt::TagByte* tagItemCount = nbtItemData->getValueAt<nbt::TagByte>("Count");
        if (tagItemCount)
        {
            stackSize = tagItemCount->getValue();
        }
        nbt::TagCompound* specialData = nbtItemData->getValueAt<nbt::TagCompound>("tag");
        if (specialData)
        {
            this->specialData = dynamic_cast<nbt::TagCompound*>(specialData->clone());
        }

    }
}

ItemStack::~ItemStack()
{
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

i_stackSize ItemStack::getStackSize() const
{
    return stackSize;
}

void ItemStack::setStackSize(i_stackSize stackSize)
{
    this->stackSize = stackSize;
}

const Item* ItemStack::getItem() const
{
    if (itemId >= ITEM_COUNT)
        return nullptr;
    return ItemList::Instance().getItem(itemId);
}

i_stackSize ItemStack::GetMaxStackSize() const
{
    const Item* item = getItem();
    if (item == nullptr)
    {
        return 0;
    }
    return item->getMaxStackSize();
}

bool ItemStack::IsStackable(const ItemStack* otherStack) const
{
    return otherStack->itemId == itemId && otherStack->itemData == itemData;
}

bool ItemStack::IsStackable(i_item otherItemId, i_damage otherItemData) const
{
    return otherItemId == itemId && otherItemData == itemData;
}


bool ItemStack::Full() const
{
    const Item* item = getItem();
    if (item == nullptr)
        return true;
    return item->getMaxStackSize() <= stackSize;
}

nbt::TagCompound* ItemStack::GetSpecialData() const
{
    return specialData;
}

} /* namespace Inventory */
