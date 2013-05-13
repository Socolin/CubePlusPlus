#include "TileEntityRecordPlayer.h"

namespace Block
{

TileEntityRecordPlayer::TileEntityRecordPlayer(int blockX, i_height blockY, int blockZ)
    : TileEntity(TILEENTITY_TYPE_JUKEBOX, blockX, blockY, blockZ)
    , Record(0)
    , recordSlot(1)
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

void TileEntityRecordPlayer::GetDataPacket(Network::NetworkPacket& /*packet*/)
{
}

bool TileEntityRecordPlayer::HasNetworkData()
{
    return false;
}

void TileEntityRecordPlayer::SetRecordItem(World::World* world, int x, i_height y, int z, Inventory::ItemStack* item)
{
    if (item != nullptr)
    {
        Record = item->getItemId();
        world->PlaySoundOrParticleEffect(x, y, z, 1005 /*TODO enum*/, Record, false, 5);
    }
    recordSlot.ClearAndSetSlot(0, item);
}

Inventory::Inventory& TileEntityRecordPlayer::GetRecordItem()// TODO use const ref, and move extern operation on this, into the class...
{
    return recordSlot;
}

} /* namespace Block */
