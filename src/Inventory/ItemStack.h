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

    ItemStack* Copy();
    int getItemData() const;
    void setItemData(int itemData);
    int getItemId() const;
    void setItemId(int itemId);
    int getStackSize() const;
    void setStackSize(int stackSize);
    void setItem(int id, int stackSize, int itemData);
    const Item* getItem() const;
    int GetMaxStackSize();
    bool IsSoftEqual(const ItemStack* otherStack) const;
private:
    int itemId;
    int stackSize;
    int itemData;
};

} /* namespace Inventory */
#endif /* ITEMSTACK_H_ */
