#ifndef TILEENTITYFURNACE_H_
#define TILEENTITYFURNACE_H_

#include "TileEntity.h"

namespace Inventory
{
    class Inventory;
}
namespace Block
{

class TileEntityFurnace: public TileEntity
{
public:
    TileEntityFurnace(int blockX, i_height blockY, int blockZ);
    virtual ~TileEntityFurnace();

    virtual void UpdateTick();
    virtual bool NeedUpdate();
    virtual void GetDataPacket(Network::NetworkPacket& packet);
    virtual bool HasNetworkData();
    virtual Inventory::Inventory* GetInventory();
private:
    Inventory::Inventory* inventory;
    int fuel;
    int progress;
};

} /* namespace Block */
#endif /* TILEENTITYFURNACE_H_ */
