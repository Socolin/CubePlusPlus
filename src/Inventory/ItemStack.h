#ifndef ITEMSTACK_H_
#define ITEMSTACK_H_

#include "ItemList.h"

namespace Inventory
{

class ItemStack
{
public:
    ItemStack(int id, int stackSize, int itemData);
    virtual ~ItemStack();

    /**
     * Do a copy of item stack
     * @return
     */
    ItemStack* Copy() const;

    /**
     * Get item data/damage
     * @return value of data
     */
    i_damage getItemData() const;

    /**
     * Set item data
     * @param itemData
     */
    void setItemData(i_damage itemData);

    /**
     * Return item Id
     * @return item id
     */
    i_item getItemId() const;

    /**
     * Return number of item in stack
     * @return
     */
    char getStackSize() const;

    /**
     * Change number of item ins tack
     * @param stackSize
     */
    void setStackSize(char stackSize);

    /**
     * Get pointer to item, usefull to use script
     * @return
     */
    const Item* getItem() const;

    /**
     * Get max stack size from item data
     * @return max stack size
     */
    int GetMaxStackSize() const;

    /**
     * Check if itemId && itemData are equals
     * TODO: check also nbt data when it will be possible
     * @param otherStack
     * @return true if two item can be stackable
     */
    bool IsSoftEqual(const ItemStack* otherStack) const;
private:
    i_item itemId;
    i_damage itemData;
    char stackSize;
};

} /* namespace Inventory */
#endif /* ITEMSTACK_H_ */
