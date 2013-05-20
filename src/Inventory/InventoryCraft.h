#ifndef INVENTORYCRAFT_H_
#define INVENTORYCRAFT_H_

#include "Inventory.h"

namespace Inventory
{

class InventoryCraft : public Inventory
{
public:
    InventoryCraft(int width, int height);
    virtual ~InventoryCraft();
    int GetResultSlotId();

    const ItemStack* LookSlot(int x, int y) const;
    virtual bool CanPlayerPlaceItemAt(i_slot slotId) override;

    void PerformCraftChecking();
private:
    int width;
    int height;
    int top;
    int left;
};

} /* namespace Inventory */
#endif /* INVENTORYCRAFT_H_ */
