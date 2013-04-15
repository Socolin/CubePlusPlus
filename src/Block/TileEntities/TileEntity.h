#ifndef TILEENTITY_H_
#define TILEENTITY_H_

#include "Util/types.h"
#include "Network/NetworkPacket.h"

namespace Block
{

#define TILEENTITY_KEY(x,y,z) (y << 8 | z << 4 | x)

enum eTileEntityType
{
    TILEENTITY_TYPE_NOTE,
    TILEENTITY_TYPE_JUKEBOX,
};

class TileEntity
{
public:
    TileEntity(eTileEntityType type, int blockX, i_height blockY, int blockZ);
    virtual ~TileEntity();

    virtual void UpdateTick() = 0;
    virtual bool NeedUpdate() = 0;
    virtual void GetDataPacket(Network::NetworkPacket& packet) = 0;
    virtual bool HasNetworkData() = 0;
    eTileEntityType getType() const;

    // Add tickFrequence ?
protected:
    eTileEntityType type;
    int blockX;
    i_height blockY;
    int blockZ;
};

} /* namespace Block */
#endif /* TILEENTITY_H_ */
