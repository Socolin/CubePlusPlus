#include "InventoryPlayer.h"

namespace Inventory
{

InventoryPlayer::InventoryPlayer()
    : handSlot(0)
{
    // TODO Auto-generated constructor stub

}

InventoryPlayer::~InventoryPlayer()
{
    // TODO Auto-generated destructor stub
}

ItemStack& InventoryPlayer::GetSlot(int slotId)
{
    return slot[slotId];
}

void InventoryPlayer::SetSlot(int slotId, const ItemStack& itemStack)
{
    slot[slotId] = itemStack;
}

void InventoryPlayer::setHandSlot(int slotId)
{
    handSlot = slotId;
}

int InventoryPlayer::getHandSlotId()
{
    return 36 + (handSlot % 9);
}

ItemStack& InventoryPlayer::GetItemInHand()
{
    return slot[36 + (handSlot % 9)];
}

} /* namespace Inventory */
