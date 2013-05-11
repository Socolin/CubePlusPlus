#include "TileEntity.h"

namespace Block
{

TileEntity::TileEntity(eTileEntityType type, int blockX, i_height blockY, int blockZ)
    : type(type), blockX(blockX), blockY(blockY), blockZ(blockZ)
{
}

TileEntity::~TileEntity()
{
}

Inventory::Inventory* TileEntity::GetInventory()
{
    return nullptr;
}

eTileEntityType TileEntity::getType() const
{
    return type;
}

} /* namespace Block */
