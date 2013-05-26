#include "TileEntityEnderChest.h"

#include "World/World.h"
#include "Util/FloatUtil.h"

namespace Block
{


TileEntityEnderChest::TileEntityEnderChest(World::World* world, int blockX, i_height blockY, int blockZ)
    : TileEntity(TILEENTITY_TYPE_ENDERCHEST, world, blockX, blockY, blockZ)
    , countUser(0)
{
}

TileEntityEnderChest::~TileEntityEnderChest()
{
}

void TileEntityEnderChest::UpdateTick()
{
}

bool TileEntityEnderChest::NeedUpdate()
{
    return false;
}

void TileEntityEnderChest::GetDataPacket(Network::NetworkPacket& /*packet*/)
{
}

bool TileEntityEnderChest::HasNetworkData()
{
    return false;
}

void TileEntityEnderChest::NotifyPlayerUse(int action)
{
    switch(action)
    {
    case TILEENTITY_PLAYER_OPEN:

        if (countUser == 0)
        {
            world->PlayBlockAction(blockX, blockY, blockZ, 1, 1, world->GetBlockId(blockX, blockY, blockZ), 4);
            world->PlaySound(blockX, blockY, blockZ, L"random.chestopen", 0.5f, 0.9f - Util::randFloat(0.1F), 4);
        }
        countUser++;
        break;
    case TILEENTITY_PLAYER_CLOSE:
        countUser--;
        if (countUser == 0)
        {
            world->PlayBlockAction(blockX, blockY, blockZ, 1, 0, world->GetBlockId(blockX, blockY, blockZ), 4);
            world->PlaySound(blockX, blockY, blockZ, L"random.chestclosed", 0.5f, 0.9f - Util::randFloat(0.1F), 4);
        }
        break;
    default:
        break;
    }
}

TileEntity* TileEntityEnderChest::Create(World::World* world, int blockX, i_height blockY, int blockZ)
{
    return new TileEntityEnderChest(world, blockX, blockY, blockZ);
}

void TileEntityEnderChest::Load(nbt::TagCompound* /*nbtData*/)
{
}

void TileEntityEnderChest::Save(nbt::TagCompound* /*nbtData*/)
{
}

const char* TileEntityEnderChest::GetName()
{
    return "EnderChest";
}

} /* namespace Block */
