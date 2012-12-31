#ifndef INVENTORYPLAYER_H_
#define INVENTORYPLAYER_H_

#include "Inventory.h"
#include "ItemStack.h"

#define PLAYER_INVENTORY_SIZE 100

namespace Inventory
{

class InventoryPlayer : public Inventory
{
public:
    InventoryPlayer();
    virtual ~InventoryPlayer();

    virtual ItemStack& GetSlot(int slotId);
    virtual void SetSlot(int slotId, const ItemStack& itemStack);

    void setHandSlot(int slotId);
    int getHandSlotId();
    ItemStack& GetItemInHand();

private:
    ItemStack slot[PLAYER_INVENTORY_SIZE];
    int handSlot;
};

} /* namespace Inventory */
#endif /* INVENTORYPLAYER_H_ */
