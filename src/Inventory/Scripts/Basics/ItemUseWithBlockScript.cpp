#include "ItemUseWithBlockScript.h"
#include "Block/Block.h"
#include "Block/BlockConstants.h"
#include "Block/BlockList.h"
#include "Entity/EntityPlayer.h"
#include "World/World.h"
#include "World/Chunk.h"

namespace Scripting
{

ItemUseWithBlockScript::ItemUseWithBlockScript()
    : ItemScript("item_usewithblock"), UseMetadata(false)
{
}

ItemUseWithBlockScript::~ItemUseWithBlockScript()
{

}

ItemScript* ItemUseWithBlockScript::Copy()
{
    return new ItemUseWithBlockScript(*this);
}

bool ItemUseWithBlockScript::OnUseOnBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
{
	std::cout << "toto est content lui aussi" <<std::endl;
    // Try activate block
    // If not, try use item on block
    World::World* world = user->getWorld();

    int clickedBlockId = world->GetBlockId(x, y, z);
    Block::Block* clicketBlock = Block::BlockList::Instance()->blocks[clickedBlockId];
    if (clicketBlock != NULL)
    {
        return clicketBlock->UseBlock(user, x, y, z, face, item, CursorpositionX, CursorpositionY, CursorpositionZ);
    }

    return false;
}

void ItemUseWithBlockScript::InitParam(int paramId, int param)
{
	/*
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
    */
}

} /* namespace Scripting */
