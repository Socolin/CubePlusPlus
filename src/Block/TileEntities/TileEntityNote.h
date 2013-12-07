#ifndef TILEENTITYNOTE_H_
#define TILEENTITYNOTE_H_

#include "TileEntity.h"

namespace Block
{

class TileEntityNote : public TileEntity
{
public:
    TileEntityNote(World::World* world, int blockX, i_height blockY, int blockZ);
    virtual ~TileEntityNote();

    virtual void UpdateTick() override;
    virtual bool NeedUpdate() override;
    virtual void GetDataPacket(Network::NetworkPacket& packet) override;
    virtual bool HasNetworkData() override;

    virtual TileEntity* Create(World::World* world, int blockX, i_height blockY, int blockZ) override;
    virtual void Load(NBT::TagCompound* nbtData) override;
    virtual void Save(NBT::TagCompound* nbtData) const override;
    virtual const char* GetName() const override;

    int getNoteLevel();
    int nextNoteLevel();

private:
    int noteLevel;
};

} /* namespace Block */
#endif /* TILEENTITYNOTE_H_ */
