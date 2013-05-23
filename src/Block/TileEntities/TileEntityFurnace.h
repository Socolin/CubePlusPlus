#ifndef TILEENTITYFURNACE_H_
#define TILEENTITYFURNACE_H_

#include "TileEntity.h"

namespace Inventory
{
    class Inventory;
    class InventoryFurnace;
}
namespace Block
{

class TileEntityFurnace: public TileEntity
{
public:
    TileEntityFurnace(World::World* world, int blockX, i_height blockY, int blockZ);
    virtual ~TileEntityFurnace();

    virtual void UpdateTick() override;
    virtual bool NeedUpdate() override;
    virtual void GetDataPacket(Network::NetworkPacket& packet) override;
    virtual bool HasNetworkData() override;
    virtual Inventory::Inventory* GetInventory() override;

    virtual TileEntity* Create(World::World* world, int blockX, i_height blockY, int blockZ) override;
    virtual void Load(nbt::TagCompound* nbtData) override;
    virtual void Save(nbt::TagCompound* nbtData) override;
    virtual const char* GetName() override;
private:
    Inventory::InventoryFurnace* inventory;
    int fuel;
    int maxfuel;
    int progress;
};

} /* namespace Block */
#endif /* TILEENTITYFURNACE_H_ */
