#ifndef ITEMSTACK_H_
#define ITEMSTACK_H_

#include "ItemList.h"

namespace Inventory
{

class ItemStack
{
public:
    ItemStack();
    ItemStack(int id, int stackSize, int itemData);
    virtual ~ItemStack();

    int getItemData() const
    {
        return itemData;
    }

    void setItemData(int itemData)
    {
        this->itemData = itemData;
    }

    int getItemId() const
    {
        return itemId;
    }

    void setItemId(int itemId)
    {
        this->itemId = itemId;
    }

    int getStackSize() const
    {
        return stackSize;
    }

    void setStackSize(int stackSize)
    {
        this->stackSize = stackSize;
    }

    void setItem(int id, int stackSize, int itemData)
    {
        this->itemId = id;
        this->itemData = itemData;
        this->stackSize = stackSize;
    }

    const Item* getItem() const
    {
        if (itemId < 0 || itemId >= ITEM_COUNT)
            return nullptr;
        return ItemList::Instance().getItem(itemId);
    }

    bool IsEmpty() const
    {
        return itemId == -1;
    }
    void Clear()
    {
        itemId = -1;
    }
private:
    int itemId;
    int stackSize;
    int itemData;
};

} /* namespace Inventory */
#endif /* ITEMSTACK_H_ */
