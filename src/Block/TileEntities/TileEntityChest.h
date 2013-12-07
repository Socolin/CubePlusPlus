#ifndef TILEENTITYCHEST_H_
#define TILEENTITYCHEST_H_

#include "TileEntity.h"

namespace Block
{

class TileEntityChest : public TileEntity
{
public:
    enum ePlayerUseAction
    {
        TILEENTITY_PLAYER_OPEN,
        TILEENTITY_PLAYER_CLOSE,
    };
public:
    TileEntityChest(World::World* world, int blockX, i_height blockY, int blockZ);
    virtual ~TileEntityChest();

    virtual void UpdateTick() override;
    virtual bool NeedUpdate() override;
    virtual void GetDataPacket(Network::NetworkPacket& packet) override;
    virtual bool HasNetworkData() override;
    virtual Inventory::Inventory* GetInventory() override;

    virtual void NotifyPlayerUse(int action) override;

    virtual TileEntity* Create(World::World* world, int blockX, i_height blockY, int blockZ) override;
    virtual void Load(NBT::TagCompound* nbtData) override;
    virtual void Save(NBT::TagCompound* nbtData) const override;
    virtual const char* GetName() const override;

    Inventory::Inventory* GetSecondInventory();
private:
    Inventory::Inventory* inventory;
};

} /* namespace Block */
#endif /* TILEENTITYCHEST_H_ */
