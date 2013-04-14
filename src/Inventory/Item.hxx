#ifndef ITEM_HXX_
#define ITEM_HXX_

namespace Inventory
{
unsigned short Item::getContainerId() const
{
    return containerId;
}

bool Item::isHasSubType() const
{
    return hasSubType;
}

unsigned short Item::getItemId() const
{
    return itemId;
}

unsigned int Item::getMaxDamage() const
{
    return maxDamage;
}

unsigned int Item::getMaxStackSize() const
{
    return maxStackSize;
}

} /* namespace Inventory */

#endif /* ITEM_HXX_ */
