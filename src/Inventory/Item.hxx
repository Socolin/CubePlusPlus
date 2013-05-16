#ifndef ITEM_HXX_
#define ITEM_HXX_

namespace Inventory
{
i_item Item::getContainerId() const
{
    return containerId;
}

bool Item::isHasSubType() const
{
    return hasSubType;
}

i_item Item::getItemId() const
{
    return itemId;
}

i_data Item::getMaxDamage() const
{
    return maxDamage;
}

unsigned char Item::getMaxStackSize() const
{
    return maxStackSize;
}

} /* namespace Inventory */

#endif /* ITEM_HXX_ */
