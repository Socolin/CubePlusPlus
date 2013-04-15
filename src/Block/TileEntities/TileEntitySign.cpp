#include "TileEntitySign.h"

#include "Network/OpcodeList.h"

namespace Block
{

TileEntitySign::TileEntitySign(int blockX, i_height blockY, int blockZ)
    : TileEntity(TILEENTITY_TYPE_SIGN, blockX, blockY, blockZ)
    , updated(false), inCache(false)
{
}

TileEntitySign::~TileEntitySign()
{
}

void TileEntitySign::UpdateTick()
{
}

bool TileEntitySign::NeedUpdate()
{
    return false;
}

bool TileEntitySign::Editable() const
{
    return !updated;
}

void TileEntitySign::GetDataPacket(Network::NetworkPacket& packet)
{
    if (!inCache)
    {
        inCache = true;
        cachePacket << (unsigned char)Network::OP_UPDATE_SIGN;
        cachePacket << blockX << (short)blockY << blockZ;
        for (int i = 0; i < 4; i++)
            cachePacket << lines[i];
    }
    packet << cachePacket;
}

bool TileEntitySign::HasNetworkData()
{
    return true;
}

void TileEntitySign::SetLine(int lineNumber,const std::wstring& line)
{
    inCache = false;
    lines[lineNumber & 0x3] = line;
}



} /* namespace Block */
