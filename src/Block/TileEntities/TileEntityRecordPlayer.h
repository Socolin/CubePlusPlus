#ifndef TILEENTITYRECPLY_H_
#define TILEENTITYRECPLY_H_

#include "TileEntity.h"

namespace Block
{

class TileEntityRecordPlayer : public TileEntity
{
public:
    TileEntityRecordPlayer();
    virtual ~TileEntityRecordPlayer();

    virtual void UpdateTick() = 0;
    virtual bool NeedUpdate() = 0;
    // Add tickFrequence ?
    void SetRecord(i_block);
    i_block GetRecord();
private:
    i_block Record;

};

} /* namespace Block */
#endif /* TILEENTITYRECPLY_H_ */
