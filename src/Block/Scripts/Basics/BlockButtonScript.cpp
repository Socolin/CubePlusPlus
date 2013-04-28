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

void BlockButtonScript::OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const
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

bool BlockButtonScript::OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const
{
    World::World* world = user->getWorld();
    i_data clickedBlockData = world->GetBlockData(x, y, z);
    if (!SCRIPT_BLOCK_BUTTON_ACTIVATED(clickedBlockData))
    {
        world->ChangeDataNotify(x, y, z, clickedBlockData | 0x8);
        notifyNeighborsUsingOrientation(world, x, y, z, clickedBlockData & 0x7);
        world->MarkBlockForUpdate(x, y, z, baseBlock->GetBlockId(), 20);
        world->PlaySound((double)x + 0.5, (double)y + 0.5, (double)z + 0.5, soundClick, 0.3, 0.6 * 63, 2);
    }
    return true;
}

bool BlockButtonScript::CanPlace(World::World* world, int x, unsigned char y, int z, char face) const
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

    const Block::Block* clickedBlock = Block::BlockList::Instance().getBlock(world->GetBlockId(x, y, z));
    if(clickedBlock != NULL)
    {
        return clickedBlock->IsOpaqueCube() && clickedBlock->IsRenderAsNormal();
    }

    return false;
}

void BlockButtonScript::OnNeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const
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

void BlockButtonScript::OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const
{
    if (SCRIPT_BLOCK_BUTTON_ACTIVATED(data))
    {
        world->ChangeDataNotify(x, y, z, data & 0x7);
        notifyNeighborsUsingOrientation(world, x, y, z, data & 0x7);
        world->PlaySound((double)x + 0.5, (double)y + 0.5, (double)z + 0.5, soundClick, 0.3, 0.6 * 63, 2);
    }
}

bool BlockButtonScript::CanProvidePower() const
{
    return true;
}

i_powerlevel BlockButtonScript::GetWeakPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const
{
    if (SCRIPT_BLOCK_BUTTON_ACTIVATED(metadata))
        return 15;
    return 0;
}

i_powerlevel BlockButtonScript::GetStrongPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const
{
    if (SCRIPT_BLOCK_BUTTON_ACTIVATED(metadata))
    {
        int orientation = metadata & 7;
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

void BlockButtonScript::notifyNeighborsUsingOrientation(World::World* world, int x, i_height y, int z, int orientation) const
{
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
    else
    {
        world->NotifyNeighborsForBlockChange(x, y - 1, z, baseBlock->GetBlockId());
    }
}

} /* namespace Scripting */
