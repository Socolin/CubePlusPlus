#include "TileEntityNote.h"

namespace Block
{

TileEntityNote::TileEntityNote():
        noteLevel(0)
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
