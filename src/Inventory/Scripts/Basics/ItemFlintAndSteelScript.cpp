#include "ItemFlintAndSteelScript.h"

#include "Block/Block.h"
#include "Block/BlockList.h"
#include "Block/BlockConstants.h"
#include "Entity/EntityPlayer.h"
#include "World/World.h"

namespace Scripting
{

ItemFlintAndSteelScript::ItemFlintAndSteelScript()
    : ItemScript("item_flint_and_steel")
{
}

ItemFlintAndSteelScript::~ItemFlintAndSteelScript()
{
}

ItemScript* ItemFlintAndSteelScript::Copy()
{
    return new ItemFlintAndSteelScript(*this);
}

ItemUseResult ItemFlintAndSteelScript::OnUseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    switch(face)
    {
    case(FACE_BOTTOM):
        --y;
        break;
    case(FACE_TOP):
        ++y;
        break;
    case(FACE_NORTH):
        --z;
        break;
    case(FACE_SOUTH):
        ++z;
        break;
    case(FACE_WEST):
        --x;
        break;
    case(FACE_EAST):
        ++x;
        break;
    default:
        break;
    }
    World::World* world = user->GetWorld();
    i_block blockId = world->GetBlockId(x, y, z);
    const Block::Block* block = Block::BlockList::getBlock(blockId);
    if(block == nullptr)
    {
        //TODO : play sound
        world->ChangeBlock(x, y, z, 51, false);
        return ItemUseResult{true, true, 1};
    }
    return ItemUseResult{false,false,0};
}


} /* namespace Scripting */
