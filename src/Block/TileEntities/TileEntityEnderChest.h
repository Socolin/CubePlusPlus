#ifndef TILEENTITYENDERCHEST_H_
#define TILEENTITYENDERCHEST_H_

#include "TileEntity.h"

namespace Block
{

class TileEntityEnderChest : public TileEntity
{
public:
    enum ePlayerUseAction
    {
        TILEENTITY_PLAYER_OPEN,
        TILEENTITY_PLAYER_CLOSE,
    };
public:
    TileEntityEnderChest(World::World* world, int blockX, i_height blockY, int blockZ);
    virtual ~TileEntityEnderChest();

    virtual void UpdateTick() override;
    virtual bool NeedUpdate() override;
    virtual void GetDataPacket(Network::NetworkPacket& packet) override;
    virtual bool HasNetworkData() override;

    virtual void NotifyPlayerUse(int action) override;

    virtual TileEntity* Create(World::World* world, int blockX, i_height blockY, int blockZ) override;
    virtual void Load(NBT::TagCompound* nbtData) override;
    virtual void Save(NBT::TagCompound* nbtData) override;
    virtual const char* GetName() override;
private:
    int countUser;

};

} /* namespace Block */
#endif /* TILEENTITYENDERCHEST_H_ */
