#ifndef INVENTORYFURNACE_H_
#define INVENTORYFURNACE_H_

#include "Inventory.h"

namespace Inventory
{
#define INVENTORY_FURNACE_SIZE 3
#define INVENTORY_FURNACE_RESULT_SLOT 2
#define INVENTORY_FURNACE_FUEL_SLOT 1
#define INVENTORY_FURNACE_INPUT_SLOT 0

class InventoryFurnace: public Inventory::Inventory
{
public:
    InventoryFurnace();
    virtual ~InventoryFurnace();

    int GetResultSlotId();
    int GetFuelSlotId();
    int GetInputSlotId();

    virtual bool CanPlayerPlaceItemAt(i_slot slotId) override;
};

} /* namespace Inventory */
#endif /* INVENTORYFURNACE_H_ */
