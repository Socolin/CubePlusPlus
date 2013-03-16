#include "TileEntityRecordPlayer.h"

namespace Block
{

TileEntityRecordPlayer::TileEntityRecordPlayer():
			Record(0),
			RecordItem(Inventory::ItemStack(0,0,0))
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

void TileEntityRecordPlayer::SetRecordItem(World::World* world, int x, i_height y, int z, Inventory::ItemStack item)
{
	RecordItem = item;
	Record = item.getItemId();

	world->PlaySoundOrParticleEffect(x, y, z, 1005 /*TODO enum*/, Record, false, 5);

	if(item.getItemId() != 0)
		world->ChangeDataNoEvent(x, y, z, 1);
	else
		world->ChangeDataNoEvent(x, y, z, 0);
}

Inventory::ItemStack TileEntityRecordPlayer::GetRecordItem()
{
	return RecordItem;
}

} /* namespace Block */
