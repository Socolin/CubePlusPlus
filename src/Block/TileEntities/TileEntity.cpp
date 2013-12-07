#include "TileEntity.h"

namespace Block
{

TileEntity::TileEntity(eTileEntityType type, World::World* world, int blockX, i_height blockY, int blockZ)
    : type(type), world(world), blockX(blockX), blockY(blockY), blockZ(blockZ)
{
}

TileEntity::~TileEntity()
{
}

Inventory::Inventory* TileEntity::GetInventory()
{
    return nullptr;
}

void TileEntity::NotifyPlayerUse(int /*action*/)
{
}

eTileEntityType TileEntity::getType() const
{
    return type;
}

int TileEntity::GetBlockX() const
{
    return blockX;
}

i_height TileEntity::GetBlockY() const
{
    return blockY;
}

int TileEntity::GetBlockZ() const
{
    return blockZ;
}

} /* namespace Block */

