#include "TileEntitySign.h"

#include <cppnbt.h>

#include "Network/OpcodeList.h"
#include "Util/StringUtil.h"

namespace Block
{

TileEntitySign::TileEntitySign(World::World* world, int blockX, i_height blockY, int blockZ)
    : TileEntity(TILEENTITY_TYPE_SIGN, world, blockX, blockY, blockZ)
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

TileEntity* TileEntitySign::Create(World::World* world, int blockX, i_height blockY, int blockZ)
{
    return new TileEntitySign(world, blockX, blockY, blockZ);
}

void TileEntitySign::Load(nbt::TagCompound* nbtData)
{
    nbt::TagString* tagText1= nbtData->getValueAt<nbt::TagString>("Text1");
    if (tagText1)
    {
        Util::StringToWString(lines[0], tagText1->getValue());
    }

    nbt::TagString* tagText2= nbtData->getValueAt<nbt::TagString>("Text2");
    if (tagText2)
    {
        Util::StringToWString(lines[1], tagText2->getValue());
    }

    nbt::TagString* tagText3= nbtData->getValueAt<nbt::TagString>("Text3");
    if (tagText3)
    {
        Util::StringToWString(lines[2], tagText3->getValue());
    }

    nbt::TagString* tagText4= nbtData->getValueAt<nbt::TagString>("Text4");
    if (tagText4)
    {
        Util::StringToWString(lines[3], tagText4->getValue());
    }}

void TileEntitySign::Save(nbt::TagCompound* /*nbtData*/)
{
}

const char* TileEntitySign::GetName()
{
    return "Sign";
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
