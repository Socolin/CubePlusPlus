#ifndef TILEENTITYRECPLY_H_
#define TILEENTITYRECPLY_H_

#include "TileEntity.h"
#include "World/World.h"
#include "Inventory/InventoryPlayer.h"

namespace Block
{

class TileEntityRecordPlayer: public TileEntity
{
public:
    TileEntityRecordPlayer(int blockX, i_height blockY, int blockZ);
    virtual ~TileEntityRecordPlayer();

    virtual void UpdateTick() override;
    virtual bool NeedUpdate() override;
    virtual void GetDataPacket(Network::NetworkPacket& packet) override;
    virtual bool HasNetworkData() override;

    void SetRecordItem(World::World* world, int x, i_height y, int z, Inventory::ItemStack* item);
    Inventory::Inventory& GetRecordItem();
private:
    i_block Record;
    Inventory::Inventory recordSlot;
};

} /* namespace Block */
#endif /* TILEENTITYRECPLY_H_ */
