#include "Block/Block.h"
#include "Block/BlockList.h"
#include "BlockStationaryLavaScript.h"
#include "World/World.h"


namespace Scripting
{

BlockStationaryLavaScript::BlockStationaryLavaScript()
    : parent_type("block_stationary_lava")
{
}

BlockStationaryLavaScript::~BlockStationaryLavaScript()
{
}

BlockScript* BlockStationaryLavaScript::Copy()
{
    return new BlockStationaryLavaScript(*this);
}

void BlockStationaryLavaScript::OnUpdateTick(World::World* world, int x, i_height y, int z, i_data /*data*/) const
{
    int randCount = rand() % 3;
    int xLoop;
    i_block blockId;

    for(xLoop = 0; xLoop < randCount; ++xLoop)
    {
        x += ( rand() % 3 ) - 1;
        ++y;
        z += ( rand() % 3 ) - 1;
        blockId = world->GetBlockId(x, y, z);
        const Block::Block* block = Block::BlockList::getBlock(blockId);
        if(blockId == 0)
        {
            if(isFlammable(world, x - 1, y, z) || isFlammable(world, x + 1, y, z) || isFlammable(world, x, y, z - 1) || isFlammable(world, x, y, z + 1) || isFlammable(world, x, y - 1, z) || isFlammable(world, x, y + 1, z))
            {
                world->ChangeBlock(x, y, z, 51, false); //FIXME : Remove magic number ? (Fire block ID)
                return;
            }
        }
        else if(block != nullptr)
        {
            if(block->GetMaterial().BlocksMovement())
            {
                return;
            }
        }
    }

    if(randCount == 0)
    {
        xLoop = x;
        blockId = z;

        for(int i = 0; i < 3; ++i)
        {
            x = xLoop + ( rand() % 3 ) - 1;
            z = blockId + ( rand() % 3 ) - 1;
            i_block topBlockId = world->GetBlockId(x, y + 1, z);
            const Block::Block* topBlock = Block::BlockList::getBlock(topBlockId);
            if(topBlock == nullptr && isFlammable(world, x, y, z))
            {
                world->ChangeBlock(x, y + 1, z, 51, false); //FIXME : Remove magic number ? (Fire block ID)
            }
        }
    }
}



bool BlockStationaryLavaScript::isFlammable(World::World* world, int x, i_height y, int z) const
{
    i_block blockId = world->GetBlockId(x, y, z);
    const Block::Block* block = Block::BlockList::getBlock(blockId);
    if(block != nullptr)
    {
        return (block->GetMaterial().isCanBurn());
    }
    return false;
}

} /* namespace Scripting */
