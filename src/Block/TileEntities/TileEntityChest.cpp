#include "TileEntityChest.h"

#include "Block/BlockConstants.h"
#include "Inventory/Inventory.h"
#include "World/World.h"

namespace Block
{

TileEntityChest::TileEntityChest(World::World* world, int blockX, i_height blockY, int blockZ)
    : TileEntity(TILEENTITY_TYPE_CHEST, world, blockX, blockY, blockZ)
{
    inventory = new Inventory::Inventory(27);
}

TileEntityChest::~TileEntityChest()
{
    inventory->CloseInventoryForDelete();
    delete inventory;
}

void TileEntityChest::UpdateTick()
{
}

bool TileEntityChest::NeedUpdate()
{
    return false;
}

void TileEntityChest::GetDataPacket(Network::NetworkPacket& /*packet*/)
{
}

bool TileEntityChest::HasNetworkData()
{
    return false;
}

Inventory::Inventory* TileEntityChest::GetInventory()
{
    return inventory;
}

Inventory::Inventory* TileEntityChest::GetSecondInventory()
{
    i_block baseBlockId = world->GetBlockId(blockX, blockY, blockZ);
    FOR_EACH_SIDE_XZ(blockX, blockZ, blockSide)
        i_block blockId = world->GetBlockId(blockSideX, blockY, blockSideZ);
        if (blockId == baseBlockId)
        {
            TileEntity* tileEntity = world->GetTileEntity(blockSideX, blockY, blockSideZ);
            if (tileEntity == nullptr)
                return nullptr;
            return tileEntity->GetInventory();
        }
        // Check double chest
    END_FOR_EACH_SIDE
    return nullptr;
}

} /* namespace Block */
