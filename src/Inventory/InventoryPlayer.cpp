#include "InventoryPlayer.h"

namespace Inventory
{

InventoryPlayer::InventoryPlayer()
    : Inventory(PLAYER_INVENTORY_SIZE, INVENTORY_TYPE_PLAYER_HANDS)
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
    return handSlot % 9;
}

} /* namespace Inventory */
