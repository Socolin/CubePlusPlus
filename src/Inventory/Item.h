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

    bool UseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const;
    bool Use(World::EntityPlayer* user) const;

    inline unsigned short getContainerId() const;
    inline bool isHasSubType() const;
    inline unsigned short getItemId() const;
    inline unsigned int getMaxDamage() const;
    inline unsigned int getMaxStackSize() const;

private:
    unsigned short itemId;
    unsigned int maxStackSize;
    unsigned int maxDamage;
    bool hasSubType;
    // For bucket, when crafting, we use content (this item) and take back container, for example a bucket
    unsigned short containerId;
    Scripting::ItemScript* script;
};

} /* namespace Inventory */

#include "Item.hxx"

#endif /* ITEM_H_ */
