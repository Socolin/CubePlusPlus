#include "ItemStack.h"

namespace Inventory
{

ItemStack::ItemStack(int id, int stackSize, int itemData)
    : itemId(id), stackSize(stackSize), itemData(itemData)
{

}

ItemStack::ItemStack()
    : itemId(-1), stackSize(0), itemData(0)
{

}

ItemStack::~ItemStack()
{
    // TODO Auto-generated destructor stub
}

} /* namespace Inventory */
