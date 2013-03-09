#include "TileEntityRecordPlayer.h"

namespace Block
{

TileEntityRecordPlayer::TileEntityRecordPlayer():
			Record(0)
{
}

TileEntityRecordPlayer::~TileEntityRecordPlayer()
{
}

void TileEntityRecordPlayer::UpdateTick()
{
}

bool TileEntityRecordPlayer::NeedUpdate()
{
    return false;
}

void TileEntityRecordPlayer::SetRecord(i_block id_disk)
{
	Record = id_disk;
}


i_block TileEntityRecordPlayer::GetRecord()
{
	return Record;
}


} /* namespace Block */
