#include "ItemBlockScript.h"
#include "Block/Block.h"
#include "Block/BlockConstants.h"
#include "Block/BlockList.h"
#include "Entity/EntityPlayer.h"
#include "World/World.h"
#include "World/Chunk.h"

namespace Scripting
{

ItemBlockScript::ItemBlockScript()
    : ItemScript("item_block"), AssociatedBlockId(0), UseMetadata(false)
{
}

ItemBlockScript::~ItemBlockScript()
{

}

ItemScript* ItemBlockScript::Copy()
{
    return new ItemBlockScript(*this);
}

bool ItemBlockScript::OnUseOnBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
{
    // Try activate block
    // If not, try use item on block
    World::World* world = user->getWorld();

    int clickedBlockId = world->GetBlockId(x, y, z);
    Block::Block* clicketBlock = Block::BlockList::Instance()->blocks[clickedBlockId];
    if (clicketBlock != NULL && clicketBlock->getMaterial().isReplacable())
    {
        if (face == FACE_NONE)
            return false;
    }
    else
    {
        switch (face)
        {
        case FACE_BOTTOM: // -Y
            y--;
            break;
        case FACE_TOP: // +Y
            y++;
            break;
        case FACE_NORTH: // -Z
            z--;
            break;
        case FACE_SOUTH: // +Z
            z++;
            break;
        case FACE_WEST: // -X
            x--;
            break;
        case FACE_EAST: // +X
            x++;
            break;
        case FACE_NONE:
            return false;
        };
    }

    i_block currentBlock = world->GetBlockId(x, y, z);
    if (currentBlock != 0)
    {
        Block::Block* block = Block::BlockList::Instance()->blocks[AssociatedBlockId];
        if (block && !block->getMaterial().isReplacable())
        {
            return false;
        }
    }


    i_data metadata = 0;
    i_block blockId = AssociatedBlockId;

    if (UseMetadata)
        metadata = item.getItemData() & 0xf;

    Block::Block* block = Block::BlockList::Instance()->blocks[AssociatedBlockId];
    if (block && block->CanPlace(user->getWorld(), x, y, z, face))
    {
        block->OnBlockPlace(user, x, y, z,face, blockId, metadata, CursorpositionX, CursorpositionY, CursorpositionZ);
        world->ChangeBlock(x, y, z, blockId, metadata, true);
        return true;
    }
    else
    {
        user->ResetBlock(x, y, z);
    }
    return false;
}

void ItemBlockScript::InitParam(int paramId, int param)
{
    switch (paramId)
    {
    case SCRIPTINGPARAM_ITEM_BLOCK_BLOCKID:
        AssociatedBlockId = param;
        if (AssociatedBlockId > BLOCK_COUNT)
            AssociatedBlockId = 0;
        break;
    case SCRIPTINGPARAM_ITEM_BLOCK_USEMETADATA:
        UseMetadata = param != 0;
        break;
    default:
        std::cerr << "BAD PARAMETER ID: " << paramId << std::endl;
        break;
    }
}

} /* namespace Scripting */
