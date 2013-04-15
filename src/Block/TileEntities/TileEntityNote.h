#ifndef TILEENTITYNOTE_H_
#define TILEENTITYNOTE_H_

#include "TileEntity.h"

namespace Block
{

class TileEntityNote : public TileEntity
{
public:
    TileEntityNote(int blockX, i_height blockY, int blockZ);
    virtual ~TileEntityNote();

    virtual void UpdateTick() override;
    virtual bool NeedUpdate() override;
    virtual void GetDataPacket(Network::NetworkPacket& packet) override;
    virtual bool HasNetworkData() override;

    int getNoteLevel();
    int nextNoteLevel();

private:
    int noteLevel;
};

} /* namespace Block */
#endif /* TILEENTITYNOTE_H_ */
