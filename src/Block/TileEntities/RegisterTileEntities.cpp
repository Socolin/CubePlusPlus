#include "RegisterTileEntities.h"

#include "TileEntityManager.h"
#include "TileEntityChest.h"
#include "TileEntityEnderChest.h"
#include "TileEntityFurnace.h"
#include "TileEntityNote.h"
#include "TileEntityRecordPlayer.h"
#include "TileEntitySign.h"

namespace Block
{

void RegisterTileEntities()
{
    TileEntityManager& tileEntity = TileEntityManager::Instance();
    tileEntity.RegisterTileEntity(new TileEntityChest(nullptr, 0, 0, 0));
    tileEntity.RegisterTileEntity(new TileEntityEnderChest(nullptr, 0, 0, 0));
    tileEntity.RegisterTileEntity(new TileEntityFurnace(nullptr, 0, 0, 0));
    tileEntity.RegisterTileEntity(new TileEntityNote(nullptr, 0, 0, 0));
    tileEntity.RegisterTileEntity(new TileEntityRecordPlayer(nullptr, 0, 0, 0));
    tileEntity.RegisterTileEntity(new TileEntitySign(nullptr, 0, 0, 0));
}

} /* namespace Block */
