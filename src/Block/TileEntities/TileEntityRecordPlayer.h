#ifndef TILEENTITYRECPLY_H_
#define TILEENTITYRECPLY_H_

#include "TileEntity.h"
#include "World/World.h"
#include "Inventory/InventoryPlayer.h"

namespace Block
{

class TileEntityRecordPlayer : public TileEntity
{
public:
    TileEntityRecordPlayer();
    virtual ~TileEntityRecordPlayer();

    virtual void UpdateTick() override;
    virtual bool NeedUpdate() override;

    void SetRecordItem(World::World* world, int x, i_height y, int z, Inventory::ItemStack item);
    Inventory::ItemStack GetRecordItem();
private:
    i_block Record;
    Inventory::ItemStack RecordItem;
};

} /* namespace Block */
#endif /* TILEENTITYRECPLY_H_ */
