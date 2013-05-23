#include "TileEntityNote.h"

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

void TileEntityNote::Load(nbt::TagCompound* nbtData)
{
    /*FIXME*/
}

void TileEntityNote::Save(nbt::TagCompound* nbtData)
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
