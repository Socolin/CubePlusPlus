#include "TileEntityFurnace.h"

#include "Inventory/InventoryFurnace.h"

namespace Block
{

TileEntityFurnace::TileEntityFurnace(int blockX, i_height blockY, int blockZ)
    : TileEntity(TILEENTITY_TYPE_FURNACE, blockX, blockY, blockZ)
    , fuel(0), progress(0)
{
    inventory = new Inventory::InventoryFurnace();
}

TileEntityFurnace::~TileEntityFurnace()
{
    //TODO: close inventory for player
    delete inventory;
}

void TileEntityFurnace::UpdateTick()
{
}

bool TileEntityFurnace::NeedUpdate()
{
    return true;
}

void TileEntityFurnace::GetDataPacket(Network::NetworkPacket& /*packet*/)
{
}

bool TileEntityFurnace::HasNetworkData()
{
    return false;
}

Inventory::Inventory* TileEntityFurnace::GetInventory()
{
    return inventory;
}

} /* namespace Block */
