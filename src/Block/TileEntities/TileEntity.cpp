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

} /* namespace Block */
