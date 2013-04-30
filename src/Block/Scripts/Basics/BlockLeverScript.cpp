#include "BlockLeverScript.h"

#include "Block/BlockConstants.h"
#include "Entity/EntityPlayer.h"
#include "Util/AssertUtil.h"
#include "World/Chunk.h"
#include "World/World.h"

namespace Scripting
{

BlockLeverScript::BlockLeverScript() :
        BlockScript("block_lever")
{
}

BlockLeverScript::~BlockLeverScript()
{
}

BlockScript* BlockLeverScript::Copy()
{
    return new BlockLeverScript(*this);
}

void BlockLeverScript::OnBlockPlacedBy(World::EntityPlayer* player, int /*x*/, i_height /*y*/, int /*z*/, int face, i_block& /*blockId*/, i_data& data, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    switch (face)
    {
     case FACE_BOTTOM: // -Y
         data = 0;
         break;
     case FACE_TOP: // +Y
     {
         data = 5;
         int playerRotation = (int)(std::floor((player->getYaw() * 4.0 / 360.0) + 0.5)) & 3;
         if (playerRotation == 1 || playerRotation == 3)
             data++;
         break;
     }
     case FACE_NORTH: // -Z
         data = 4;
         break;
     case FACE_SOUTH: // +Z
         data = 3;
         break;
     case FACE_WEST: // -X
         data = 2;
         break;
     case FACE_EAST: // +X
         data = 1;
         break;
     case FACE_NONE:
         return;
     default:
         AssertSwitchBadDefault(face)
         break;
     }
}

bool BlockLeverScript::OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char /*face*/, Inventory::ItemStack& /*item*/, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    World::World* world = user->getWorld();
    i_data clickedBlockData = world->GetBlockData(x, y, z);
    world->ChangeDataNotify(x, y, z, clickedBlockData ^ 0x8);

    notifyNearBlock(world, x, y, z, clickedBlockData);
    return true;
}


bool BlockLeverScript::CanProvidePower() const
{
    return true;
}

i_powerlevel BlockLeverScript::GetWeakPowerLevel(World::World* /*world*/, int /*x*/, i_height /*y*/, int /*z*/, int /*side*/, i_data metadata) const
{
    if (metadata & 8)
        return 15;
    return 0;
}

i_powerlevel BlockLeverScript::GetStrongPowerLevel(World::World* /*world*/, int /*x*/, i_height /*y*/, int /*z*/, int side, i_data metadata) const
{
    if ((metadata & 8) == 0)
    {
        return 0;
    }
    else
    {
        int orientation = metadata & 7;
        if (orientation == 0 && side == 0)
            return 15;
        if (orientation == 7 && side == 0)
            return 15;
        if (orientation == 6 && side == 1)
            return 15;
        if (orientation == 5 && side == 1)
            return 15;
        if (orientation == 4 && side == 2)
            return 15;
        if (orientation == 3 && side == 3)
            return 15;
        if (orientation == 2 && side == 4)
            return 15;
        if (orientation == 1 && side == 5)
            return 15;
    }
    return 0;
}

void BlockLeverScript::OnDestroy(World::World* world, int x, i_height y, int z, i_data data) const
{
    notifyNearBlock(world, x, y, z, data);
}


void BlockLeverScript::notifyNearBlock(World::World* world, int x, i_height y, int z, i_data data) const
{
    int orientation = data & 7;
    if (orientation == 1)
    {
       world->NotifyNeighborsForBlockChange(x - 1, y, z, baseBlock->GetBlockId());
    }
    else if (orientation == 2)
    {
       world->NotifyNeighborsForBlockChange(x + 1, y, z, baseBlock->GetBlockId());
    }
    else if (orientation == 3)
    {
       world->NotifyNeighborsForBlockChange(x, y, z - 1, baseBlock->GetBlockId());
    }
    else if (orientation == 4)
    {
       world->NotifyNeighborsForBlockChange(x, y, z + 1, baseBlock->GetBlockId());
    }
    else if (orientation != 5 && orientation != 6)
    {
       if (orientation == 0 || orientation == 7)
       {
           world->NotifyNeighborsForBlockChange(x, y + 1, z, baseBlock->GetBlockId());
       }
    }
    else
    {
       world->NotifyNeighborsForBlockChange(x, y - 1, z, baseBlock->GetBlockId());
    }
}

} /* namespace Scripting */
