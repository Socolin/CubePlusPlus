#include "ItemSignScript.h"

#include "Block/Block.h"
#include "Block/BlockConstants.h"
#include "Block/BlockList.h"
#include "Entity/EntityPlayer.h"
#include "Util/AssertUtil.h"
#include "Util/BlockUtil.h"
#include "World/World.h"
#include "World/Chunk.h"

namespace Scripting
{

ItemSignScript::ItemSignScript()
    : ItemScript("item_sign"), groundBlockId(0), wallBlockId(0)
{
}

ItemSignScript::~ItemSignScript()
{

}

ItemScript* ItemSignScript::Copy()
{
    return new ItemSignScript(*this);
}

World::ItemUseResult ItemSignScript::OnUseOnBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const
{
    if (face == FACE_BOTTOM)
        return World::ItemUseResult{false, false, 0};

    World::World* world = user->GetWorld();

    int clickedBlockId = world->GetBlockId(x, y, z);
    const Block::Block* clicketBlock = Block::BlockList::getBlock(clickedBlockId);
    if (clicketBlock != NULL && clicketBlock->GetMaterial().isReplacable())
    {
        if (face == FACE_NONE)
            return World::ItemUseResult{false, false, 0};
    }
    else
    {
        if (!Util::UpdateXYZForSide(face, x, y, z))
            return World::ItemUseResult{false, false, 0};
    }

    i_block currentBlockId = world->GetBlockId(x, y, z);
    if (currentBlockId != 0)
    {
        const Block::Block* currentBlock = Block::BlockList::getBlock(currentBlockId);
        if (currentBlock && !currentBlock->GetMaterial().isReplacable())
        {
            return World::ItemUseResult{false, false, 0};
        }
    }

    i_block blockId = wallBlockId;
    if (face == FACE_TOP)
        blockId = groundBlockId;

    i_data metadata = 0;

    const Block::Block* block = Block::BlockList::getBlock(blockId);
    if (block && block->CanPlace(user->GetWorld(), x, y, z, face))
    {
        block->OnBlockPlace(user, x, y, z,face, blockId, metadata, cursorPositionX, cursorPositionY, cursorPositionZ);
        world->ChangeBlock(x, y, z, blockId, metadata, true);
        return World::ItemUseResult{true, false, 1};
    }
    else
    {
        user->ResetBlock(x, y, z);
    }
    return World::ItemUseResult{false, false, 0};
}

void ItemSignScript::InitParam(int paramId, int param)
{
    switch (paramId)
    {
    case SCRIPTINGPARAM_ITEM_SIGN_GROUNDBLOCKID:
        groundBlockId = param;
        if (groundBlockId > BLOCK_COUNT)
            groundBlockId = 0;
        break;
    case SCRIPTINGPARAM_ITEM_SIGN_WALLBLOCKID:
        wallBlockId = param;
        if (wallBlockId > BLOCK_COUNT)
            wallBlockId = 0;
        break;
    default:
        AssertSwitchBadDefault(paramId)
        break;
    }
}

} /* namespace Scripting */
