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
    TileEntityRecordPlayer(World::World* world, int blockX, i_height blockY, int blockZ);
    virtual ~TileEntityRecordPlayer();

    virtual void UpdateTick() override;
    virtual bool NeedUpdate() override;
    virtual void GetDataPacket(Network::NetworkPacket& packet) override;
    virtual bool HasNetworkData() override;

    virtual TileEntity* Create(World::World* world, int blockX, i_height blockY, int blockZ) override;
    virtual void Load(NBT::TagCompound* nbtData) override;
    virtual void Save(NBT::TagCompound* nbtData) const override;
    virtual const char* GetName() const override;

    void SetRecordItem(World::World* world, int x, i_height y, int z, Inventory::ItemStack* item);
    Inventory::Inventory& GetRecordItem();
private:
    i_block Record;
    Inventory::Inventory recordSlot;
};

} /* namespace Block */
#endif /* TILEENTITYRECPLY_H_ */
