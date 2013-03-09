#ifndef TILEENTITYNOTE_H_
#define TILEENTITYNOTE_H_

#include "TileEntity.h"

namespace Block
{

class TileEntityNote : public TileEntity
{
public:
    TileEntityNote();
    virtual ~TileEntityNote();

    virtual void UpdateTick() override;
    virtual bool NeedUpdate() override;

    int getNoteLevel();
    int nextNoteLevel();

private:
    int noteLevel;
};

} /* namespace Block */
#endif /* TILEENTITYNOTE_H_ */
