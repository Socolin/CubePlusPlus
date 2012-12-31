#ifndef ITEMSTACK_H_
#define ITEMSTACK_H_

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

private:
    int itemId;
    int stackSize;
    int itemData;
};

} /* namespace Inventory */
#endif /* ITEMSTACK_H_ */
