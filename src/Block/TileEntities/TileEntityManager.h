#ifndef TILEENTITYMANAGER_H_
#define TILEENTITYMANAGER_H_

#include "Util/Singleton.h"

#include <string>
#include <unordered_map>

#include "Util/types.h"

namespace World
{
class World;
}
namespace Block
{

void RegisterTileEntities();

class TileEntity;
class TileEntityManager : public Util::Singleton<TileEntityManager>
{
    friend Util::Singleton<TileEntityManager>;
    TileEntityManager();
public:
    virtual ~TileEntityManager();
    TileEntity* GetNewTileEntityByName(const std::string& name, World::World* world, int blockX, i_height blockY, int blockZ);
    void RegisterTileEntity(TileEntity* tileEntity);
private:
    std::unordered_map<std::string, TileEntity*> nameTileEntityList;
};

} /* namespace Block */
#endif /* TILEENTITYMANAGER_H_ */
