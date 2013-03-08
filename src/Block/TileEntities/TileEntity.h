#ifndef TILEENTITY_H_
#define TILEENTITY_H_

#include "Util/types.h"

namespace Block
{
#define TILEENTITY_KEY(x,y,z) (y << 8 | z << 4 | x)

class TileEntity
{
public:
    TileEntity();
    virtual ~TileEntity();

    virtual void UpdateTick() = 0;
    virtual bool NeedUpdate() = 0;
    // Add tickFrequence ?

};

} /* namespace Block */
#endif /* TILEENTITY_H_ */
