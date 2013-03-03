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

bool ItemBlockScript::OnUseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, Inventory::ItemStack& item)
{
    // Try activate block
    // If not, try use item on block

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

    World::World* world = user->getWorld();
    World::Chunk* chunk = world->GetChunk(x >> 4, z >> 4);
    short metadata = 0;
    if (UseMetadata)
        metadata = item.getItemData();
    short blockId = AssociatedBlockId;

    if (AssociatedBlockId < BLOCK_COUNT)
    {
        Block::Block* block = Block::BlockList::Instance()->blocks[AssociatedBlockId];
        if (block && block->CanPlace(user->getWorld(), x, y, z, metadata))
        {
            block->OnBlockPlace(user, x, y, z,face, blockId, metadata);
            chunk->ChangeBlock(x & 0xf, y, z & 0xf, blockId, metadata);
        }
        return true;
    }
    return false;
}

void ItemBlockScript::InitParam(int paramId, int param)
{
    switch (paramId)
    {
    case SCRIPTINGPARAM_ITEM_BLOCK_BLOCKID:
        AssociatedBlockId = param;
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
