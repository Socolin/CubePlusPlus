#include "InventoryPlayer.h"

namespace Inventory
{

InventoryPlayer::InventoryPlayer()
    : Inventory(PLAYER_INVENTORY_SIZE)
    , handSlot(0)
{
}

InventoryPlayer::~InventoryPlayer()
{
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

void InventoryPlayer::SendInventoryTo(World::EntityPlayer* /*entityPlayer*/)
{
}

} /* namespace Inventory */
