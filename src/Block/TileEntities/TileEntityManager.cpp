#include "TileEntityManager.h"

#include "TileEntity.h"

namespace Block
{

TileEntityManager::TileEntityManager()
{
}

TileEntityManager::~TileEntityManager()
{
    for(auto tileEntityItr : nameTileEntityList)
    {
        delete tileEntityItr.second;
    }
    nameTileEntityList.clear();
}

TileEntity* TileEntityManager::GetNewTileEntityByName(const std::string& name, World::World* world, int blockX, i_height blockY, int blockZ)
{
    auto tileEntityItr = nameTileEntityList.find(name);
    if (tileEntityItr != nameTileEntityList.end())
    {
        TileEntity* newTileEntity = tileEntityItr->second->Create(world, blockX, blockY, blockZ);
        return newTileEntity;
    }
    return nullptr;
}

void TileEntityManager::RegisterTileEntity(TileEntity* tileEntity)
{
    nameTileEntityList[tileEntity->GetName()] = tileEntity;
}

} /* namespace Block */
