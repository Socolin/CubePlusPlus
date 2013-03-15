#include "BlockButtonScript.h"

#include "Block/BlockConstants.h"
#include "World/Chunk.h"
#include "World/World.h"
#include "Entity/EntityPlayer.h"

namespace Scripting
{

BlockButtonScript::BlockButtonScript()
    : BlockScript("block_button")
    , soundClick(L"random.click")
{
}

BlockButtonScript::~BlockButtonScript()
{
}

BlockScript* BlockButtonScript::Copy()
{
    return new BlockButtonScript(*this);
}

void BlockButtonScript::OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
{
    switch (face)
    {
     case FACE_BOTTOM: // -Y
         break;
     case FACE_TOP: // +Y
         break;
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

bool BlockButtonScript::OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
{
    World::World* world = user->getWorld();
    i_data clickedBlockData = world->GetBlockData(x, y, z);
    if (!SCRIPT_BLOCK_BUTTON_ACTIVATED(clickedBlockData))
    {
        world->ChangeDataNoEvent(x, y, z, clickedBlockData | 0x8);
        world->MarkBlockForUpdate(x, y, z, baseBlock->GetBlockId(), 20);
        world->PlaySound((double)x + 0.5, (double)y + 0.5, (double)z + 0.5, soundClick, 0.3, 0.6 * 63, 2);
    }
    return true;
}

bool BlockButtonScript::CanPlace(World::World* world, int x, unsigned char y, int z, char face)
{
    switch (face)
    {
        case FACE_NORTH: // -Z
            z++;
            break;
        case FACE_SOUTH: // +Z
            z--;
            break;
        case FACE_WEST: // -X
            x++;
            break;
        case FACE_EAST: // +X
            x--;
            break;
        default:
            return false;
            break;
    };

    Block::Block* clickedBlock = Block::BlockList::Instance()->blocks[world->GetBlockId(x, y, z)];

    if(clickedBlock != NULL)
    {
        return clickedBlock->GetIsOpaqueCube() && clickedBlock->isRenderAsNormal();
    }

    return false;
}

void BlockButtonScript::OnNeighborChange(World::World* world, int x, i_height y, int z)
{
    i_data orientation = world->GetBlockData(x, y, z);
    orientation &= 0x7;
    char face = FACE_NONE;
    switch (orientation)
    {
    case 1:
        face = FACE_EAST;
        break;
    case 2:
        face = FACE_WEST;
        break;
    case 3:
        face = FACE_SOUTH;
        break;
    case 4:
        face = FACE_NORTH;
        break;
    }

    if (!CanPlace(world, x, y, z, face))
    {
        world->DropItemstackWithRandomDirection(x + 0.5, y + 0.5, z + 0.5, Inventory::ItemStack(baseBlock->GetBlockId(), 1, 0));
        world->ChangeBlockNoEvent(x, y, z, 0, 0);
    }
}

void BlockButtonScript::OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data)
{
    if (SCRIPT_BLOCK_BUTTON_ACTIVATED(data))
    {
        world->ChangeDataNoEvent(x, y, z, data & 0x7);
        world->MarkBlockForUpdate(x, y, z, baseBlock->GetBlockId(), 20);
        world->PlaySound((double)x + 0.5, (double)y + 0.5, (double)z + 0.5, soundClick, 0.3, 0.6 * 63, 2);
    }
}

} /* namespace Scripting */
