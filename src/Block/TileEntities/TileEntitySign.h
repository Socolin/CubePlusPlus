#ifndef TILEENTITYSIGN_H_
#define TILEENTITYSIGN_H_

#include "TileEntity.h"

namespace Block
{

class TileEntitySign: public TileEntity
{
public:
    TileEntitySign(World::World* world, int blockX, i_height blockY, int blockZ);
    virtual ~TileEntitySign();
    virtual void UpdateTick() override;
    virtual bool NeedUpdate() override;
    virtual void GetDataPacket(Network::NetworkPacket& packet) override;
    virtual bool HasNetworkData() override;

    virtual TileEntity* Create(World::World* world, int blockX, i_height blockY, int blockZ) override;
    virtual void Load(NBT::TagCompound* nbtData) override;
    virtual void Save(NBT::TagCompound* nbtData) const override;
    virtual const char* GetName() const override;

    bool Editable() const;
    void SetLine(int lineNumber,const std::wstring& line);
private:
    bool updated;
    std::wstring lines[4];
    bool inCache;
    Network::NetworkPacket cachePacket;
};

} /* namespace Block */
#endif /* TILEENTITYSIGN_H_ */
