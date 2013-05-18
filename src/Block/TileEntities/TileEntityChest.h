#ifndef TILEENTITYCHEST_H_
#define TILEENTITYCHEST_H_

#include "TileEntity.h"

namespace Block
{

class TileEntityChest : public TileEntity
{
public:
    TileEntityChest(World::World* world, int blockX, i_height blockY, int blockZ);
    virtual ~TileEntityChest();

    virtual void UpdateTick();
    virtual bool NeedUpdate();
    virtual void GetDataPacket(Network::NetworkPacket& packet);
    virtual bool HasNetworkData();
    virtual Inventory::Inventory* GetInventory();

    Inventory::Inventory* GetSecondInventory();
private:
    Inventory::Inventory* inventory;
};

} /* namespace Block */
#endif /* TILEENTITYCHEST_H_ */
