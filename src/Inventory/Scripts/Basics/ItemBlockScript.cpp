#include "ItemBlockScript.h"

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

ItemUseResult ItemBlockScript::OnUseOnBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const
{
    Inventory::InventoryPlayer* handInventory = user->GetHandsInventory();
    const Inventory::ItemStack* item = handInventory->LookSlot(handInventory->getHandSlotId());
    if (item == nullptr)
        return ItemUseResult{false, false, 0};

    World::World* world = user->GetWorld();

    int clickedBlockId = world->GetBlockId(x, y, z);
    const Block::Block* clicketBlock = Block::BlockList::getBlock(clickedBlockId);
    if (clicketBlock != NULL && clicketBlock->GetMaterial().isReplacable())
    {
        if (face == FACE_NONE)
            return ItemUseResult{false, false, 0};
    }
    else
    {
        if (!Util::UpdateXYZForSide(face, x, y, z))
            return ItemUseResult{false, false, 0};
    }

    if (world->IsReadOnly())
    {
        user->ResetBlock(x, y, z);
        return ItemUseResult{false, false, 0};
    }

    i_block currentBlockId = world->GetBlockId(x, y, z);
    if (currentBlockId != 0)
    {
        const Block::Block* currentBlock = Block::BlockList::getBlock(currentBlockId);
        if (currentBlock && !currentBlock->GetMaterial().isReplacable())
        {
            return ItemUseResult{false, false, 0};
        }
    }


    i_data metadata = 0;
    i_block blockId = AssociatedBlockId;

    if (UseMetadata)
        metadata = item->getItemData() & 0xf;

    const Block::Block* block = Block::BlockList::getBlock(AssociatedBlockId);
    if (block && block->CanPlace(user->GetWorld(), x, y, z, face))
    {
        block->OnBlockPlace(user, x, y, z,face, blockId, metadata, cursorPositionX, cursorPositionY, cursorPositionZ);
        world->ChangeBlock(x, y, z, blockId, metadata, true);
        return ItemUseResult{true, false, 1};
    }
    else
    {
        user->ResetBlock(x, y, z);
    }
    return ItemUseResult{false, false, 0};
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
        AssertSwitchBadDefault(paramId)
        break;
    }
}

} /* namespace Scripting */
