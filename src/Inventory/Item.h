#ifndef ITEM_H_
#define ITEM_H_

#include "Util/types.h"

namespace World
{
class EntityPlayer;
}
namespace Scripting
{
class ItemScript;
}
namespace Inventory
{
class ItemStack;

class Item
{
public:
    /**
     * Create new item
     * @param itemId the id of item
     * @param maxStackSize the stack size of item
     * @param maxDamage the max durability of item
     * @param hasSubType if item has subtype
     * @param containerId the container item id
     * @param burningTime burning time in tick as fuel in furnace
     * @param script the item script
     */
    Item(i_item itemId, unsigned char maxStackSize, i_data maxDamage, bool hasSubType, i_item containerId, int burningTime, Scripting::ItemScript* script);

    /**
     * Destructor
     */
    virtual ~Item();

    /**
     * Use item on a block: do a right click on block while holding an item
     * @param user the player who use the item
     * @param x clicked block x
     * @param y clicked block y
     * @param z clicked block z
     * @param face clicked face
     * @param cursorPositionX position on block where player click [0;16[
     * @param cursorPositionY position on block where player click [0;16[
     * @param cursorPositionZ position on block where player click [0;16[
     * @return see ItemUseResult
     */
    ItemUseResult UseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const;

    /**
     * Use item in air: do right click, targeting no block.
     * @param user player who use item
     * @return see ItemUseResult
     */
    ItemUseResult Use(World::EntityPlayer* user) const;

    /**
     * Test if the item will be efficiency against the block
     * @param blockId block to break
     * @return true if the item is efficiency
     */
    bool CanHarvestBlock(i_block blockId) const;

    /**
     * Get
     * @param blockId
     * @return
     */
    float GetStrengthVsBlock(i_block blockId) const;

    /**
     * Get the container item, like for bucket of water, the container is bucket, so when crafting something with
     * bucket of water, only 'water' is used
     */
    inline unsigned short getContainerId() const;

    /**
     * Return true if item use data to define different item, like for charcoal and coal, so they can't be stack
     * @return if item has subtype
     */
    inline bool isHasSubType() const;

    /**
     * Return id of item
     * @return
     */
    inline i_item getItemId() const;

    /**
     * Return the max durability of item
     * @return
     */
    inline i_data getMaxDamage() const;

    /**
     * Get max item that can be stack
     * @return
     */
    inline unsigned char getMaxStackSize() const;

    inline int getBurningTime() const;


private:
    i_item itemId;
    unsigned char maxStackSize;
    i_data maxDamage;
    bool hasSubType;
    // For bucket, when crafting, we use content (this item) and take back container, for example a bucket
    i_item containerId;
    int burningTime;
    Scripting::ItemScript* script;
};

} /* namespace Inventory */

#include "Item.hxx"

#endif /* ITEM_H_ */
