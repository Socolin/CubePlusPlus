#include "TileEntityRecordPlayer.h"

namespace Block
{

TileEntityRecordPlayer::TileEntityRecordPlayer(World::World* world, int blockX, i_height blockY, int blockZ)
    : TileEntity(TILEENTITY_TYPE_JUKEBOX, world, blockX, blockY, blockZ)
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

TileEntity* TileEntityRecordPlayer::Create(World::World* world, int blockX, i_height blockY, int blockZ)
{
    return new TileEntityRecordPlayer(world, blockX, blockY, blockZ);
}

void TileEntityRecordPlayer::Load(nbt::TagCompound* /*nbtData*/)
{
    /*FIXME*/
}

void TileEntityRecordPlayer::Save(nbt::TagCompound* /*nbtData*/)
{
    /*FIXME*/
}

const char* TileEntityRecordPlayer::GetName()
{
    return "RecordPlayer";
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
