#include "BlockLeverScript.h"
#include "Block/BlockConstants.h"
#include "World/Chunk.h"
#include "World/World.h"
#include "Entity/EntityPlayer.h"

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

void BlockLeverScript::OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
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
     }
}

bool BlockLeverScript::OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
{
    World::World* world = user->getWorld();
    World::Chunk* chunk = world->GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk == nullptr)
        return 0;
    i_data clickedBlockData = chunk->getDataAt(x & 0xf, y, z & 0xf);
    chunk->ChangeData(x & 0xf, y, z & 0xf, clickedBlockData ^ 0x8);
    return true;
}

} /* namespace Scripting */
