#ifndef ITEMSTACK_H_
#define ITEMSTACK_H_

#include "ItemList.h"

namespace nbt
{
class TagCompound;
}
namespace Inventory
{

class ItemStack
{
public:
    ItemStack(int id, int stackSize, int itemData);
    ItemStack(const ItemStack& itemStack);
    ItemStack(nbt::TagCompound* itemData);
    virtual ~ItemStack();

    /**
     * Do a copy of item stack
     * @return a new itemstack
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
    i_stackSize getStackSize() const;

    /**
     * Change number of item ins tack
     * @param stackSize
     */
    void setStackSize(i_stackSize stackSize);

    /**
     * Get pointer to item, usefull to use script
     * @return
     */
    const Item* getItem() const;

    /**
     * Get max stack size from item data
     * @return max stack size
     */
    i_stackSize GetMaxStackSize() const;

    /**
     * Check if itemId && itemData are equals
     * TODO: check also nbt data when it will be possible
     *          need add equals method on nbt
     * @param otherStack
     * @return true if two item can be stackable
     */
    bool IsStackable(const ItemStack* otherStack) const;

    /**
     * Check if item is stackable with a other item
     * @param otherItemId id of other item
     * @param otherItemData damage of other item
     * @return
     */
    bool IsStackable(i_item otherItemId, i_damage otherItemData) const;

    /**
     * Check if itemStackSize == maxStackSize
     * @return true if stack has the maximum size
     */
    bool Full() const;

    nbt::TagCompound* GetSpecialData() const;

private:
    i_item itemId;
    i_damage itemData;
    i_stackSize stackSize;
    nbt::TagCompound* specialData;
};

} /* namespace Inventory */
#endif /* ITEMSTACK_H_ */
