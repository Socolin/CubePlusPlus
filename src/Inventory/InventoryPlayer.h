#ifndef INVENTORYPLAYER_H_
#define INVENTORYPLAYER_H_

#include "Inventory.h"

#define PLAYER_INVENTORY_SIZE 100 //TODO change this

namespace Inventory
{

class InventoryPlayer : public Inventory
{
public:
    InventoryPlayer();
    virtual ~InventoryPlayer();

    void setHandSlot(int slotId);
    int getHandSlotId();
    ItemStack& GetItemInHand();

    void SendInventoryTo(World::EntityPlayer* entityPlayer);
private:
    int handSlot;
};

} /* namespace Inventory */
#endif /* INVENTORYPLAYER_H_ */
