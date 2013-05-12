#include "InventoryFurnace.h"

namespace Inventory
{

InventoryFurnace::InventoryFurnace()
    : Inventory(INVENTORY_FURNACE_SIZE)
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

void InventoryFurnace::SendInventoryTo(World::EntityPlayer* /*entityPlayer*/)
{
}

} /* namespace Inventory */
