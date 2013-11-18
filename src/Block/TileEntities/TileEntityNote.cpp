#include "TileEntityNote.h"

#include <NBTField/NBTField.h>

namespace Block
{

TileEntityNote::TileEntityNote(World::World* world, int blockX, i_height blockY, int blockZ)
        : TileEntity(TILEENTITY_TYPE_NOTE, world, blockX, blockY, blockZ), noteLevel(0)
{
}

TileEntityNote::~TileEntityNote()
{
}

void TileEntityNote::UpdateTick()
{
}

bool TileEntityNote::NeedUpdate()
{
    return false;
}

void TileEntityNote::GetDataPacket(Network::NetworkPacket& /*packet*/)
{
}

bool TileEntityNote::HasNetworkData()
{
    return false;
}

TileEntity* TileEntityNote::Create(World::World* world, int blockX, i_height blockY, int blockZ)
{
    return new TileEntityNote(world, blockX, blockY, blockZ);
}

void TileEntityNote::Load(NBT::TagCompound* nbtData)
{
    NBT::TagByte* tagNote = nbtData->GetTagAs<NBT::TagByte>("note");
    if (tagNote)
    {
        noteLevel = tagNote->GetValue();
    }
}

void TileEntityNote::Save(NBT::TagCompound* /*nbtData*/)
{
    /*FIXME*/
}

const char* TileEntityNote::GetName()
{
    return "Music";
}

int TileEntityNote::getNoteLevel()
{
    return noteLevel;
}

int TileEntityNote::nextNoteLevel()
{
    noteLevel++;
    noteLevel %= 25;
    return noteLevel;
}

} /* namespace Block */
