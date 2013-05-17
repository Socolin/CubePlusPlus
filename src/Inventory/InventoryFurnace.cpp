#include "InventoryFurnace.h"

namespace Inventory
{

InventoryFurnace::InventoryFurnace()
    : Inventory(INVENTORY_FURNACE_SIZE, INVENTORY_TYPE_FURNACE)
{
}

InventoryFurnace::~InventoryFurnace()
{
}

int InventoryFurnace::GetResultSlotId()
{
    return INVENTORY_FURNACE_RESULT_SLOT;
}

int InventoryFurnace::GetFuelSlotId()
{
    return INVENTORY_FURNACE_FUEL_SLOT;
}

int InventoryFurnace::GetInputSlotId()
{
    return INVENTORY_FURNACE_INPUT_SLOT;
}

bool InventoryFurnace::CanPlayerPlaceItemAt(i_slot slotId)
{
    return slotId != INVENTORY_FURNACE_RESULT_SLOT;
}

} /* namespace Inventory */
