#include "TileEntitySign.h"

#include <NBTField/NBTField.h>

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

void TileEntitySign::Load(NBT::TagCompound* nbtData)
{
    NBT::TagString* tagText1= nbtData->GetTagAs<NBT::TagString>("Text1");
    if (tagText1)
    {
        Util::StringToWString(lines[0], tagText1->GetValue());
    }

    NBT::TagString* tagText2= nbtData->GetTagAs<NBT::TagString>("Text2");
    if (tagText2)
    {
        Util::StringToWString(lines[1], tagText2->GetValue());
    }

    NBT::TagString* tagText3= nbtData->GetTagAs<NBT::TagString>("Text3");
    if (tagText3)
    {
        Util::StringToWString(lines[2], tagText3->GetValue());
    }

    NBT::TagString* tagText4= nbtData->GetTagAs<NBT::TagString>("Text4");
    if (tagText4)
    {
        Util::StringToWString(lines[3], tagText4->GetValue());
    }}

void TileEntitySign::Save(NBT::TagCompound* nbtData) const
{
    std::string line;
    Util::WStringToString(lines[0], line);
    nbtData->AddString("Text1", line);
    Util::WStringToString(lines[1], line);
    nbtData->AddString("Text2", line);
    Util::WStringToString(lines[2], line);
    nbtData->AddString("Text3", line);
    Util::WStringToString(lines[3], line);
    nbtData->AddString("Text4", line);
}

const char* TileEntitySign::GetName() const
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
