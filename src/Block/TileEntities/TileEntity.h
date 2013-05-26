#ifndef TILEENTITY_H_
#define TILEENTITY_H_

#include "Util/types.h"
#include "Network/NetworkPacket.h"

namespace Inventory
{
class Inventory;
}
namespace nbt
{
class TagCompound;
}
namespace World
{
class World;
}
namespace Block
{

#define TILEENTITY_KEY(x,y,z) (y << 8 | z << 4 | x)

enum eTileEntityType
{
    TILEENTITY_TYPE_NOTE,
    TILEENTITY_TYPE_JUKEBOX,
    TILEENTITY_TYPE_SIGN,
    TILEENTITY_TYPE_FURNACE,
    TILEENTITY_TYPE_CHEST,
    TILEENTITY_TYPE_ENDERCHEST,
    TILEENTITY_TYPE_MAX // Keep it at end of enum
};


class TileEntity
{
public:
    TileEntity(eTileEntityType type, World::World* world, int blockX, i_height blockY, int blockZ);
    virtual ~TileEntity();

    virtual void UpdateTick() = 0;
    virtual bool NeedUpdate() = 0;
    virtual void GetDataPacket(Network::NetworkPacket& packet) = 0;
    virtual bool HasNetworkData() = 0;
    virtual Inventory::Inventory* GetInventory();
    virtual void NotifyPlayerUse(int action);
    eTileEntityType getType() const;

    virtual TileEntity* Create(World::World* world, int blockX, i_height blockY, int blockZ) = 0;
    virtual void Load(nbt::TagCompound* nbtData) = 0;
    virtual void Save(nbt::TagCompound* nbtData) = 0;

    virtual const char* GetName() = 0;
    // Add tickFrequence ?
protected:
    eTileEntityType type;
    World::World* world;
    int blockX;
    i_height blockY;
    int blockZ;
};

} /* namespace Block */
#endif /* TILEENTITY_H_ */
