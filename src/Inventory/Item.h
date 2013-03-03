#ifndef ITEM_H_
#define ITEM_H_

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
    Item(unsigned short itemId, unsigned int maxStackSize, unsigned int maxDamage, bool hasSubType, unsigned short containerId, Scripting::ItemScript* script);
    virtual ~Item();

    inline unsigned short getContainerId() const
    {
        return containerId;
    }

    inline bool isHasSubType() const
    {
        return hasSubType;
    }

    inline unsigned short getItemId() const
    {
        return itemId;
    }

    inline unsigned int getMaxDamage() const
    {
        return maxDamage;
    }

    inline unsigned int getMaxStackSize() const
    {
        return maxStackSize;
    }

    bool UseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, ItemStack& item);

private:
    unsigned short itemId;
    unsigned int maxStackSize;
    unsigned int maxDamage;
    bool hasSubType;
    // For bucket, when crafting, we use content (this item) and take back container, for example a bucket
    unsigned short containerId;
    Scripting::ItemScript* script;
};

} /* namespace Scripting */
#endif /* ITEM_H_ */
