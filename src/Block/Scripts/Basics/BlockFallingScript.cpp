#include "BlockFallingScript.h"

#include <cassert>
#include <iostream>

#include "Entity/EntityPlayer.h"
#include "Entity/EntityFallingBlock.h"
#include "Block/BlockConstants.h"
#include "World/World.h"

namespace Scripting
{

BlockFallingScript::BlockFallingScript()
    : BlockScript("block_falling")
{
}

BlockFallingScript::~BlockFallingScript()
{
}

BlockScript* BlockFallingScript::Copy()
{
    return new BlockFallingScript(*this);
}

void BlockFallingScript::OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
{
    assert(player->getWorld() != NULL);
    player->getWorld()->MarkBlockForUpdate(x, y, z, blockId, 5);
}

void BlockFallingScript::OnNeighborChange(World::World* world, int x, i_height y, int z)
{
    world->MarkBlockForUpdate(x, y, z, baseBlock->GetBlockId(), 5);
}

void BlockFallingScript::OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data)
{
    if (y == 0)
        return;
    if (isBlockTraversable(world, x, y - 1, z))
    {
        World::EntityFallingBlock* entity = new World::EntityFallingBlock((double)x + 0.5, (double)y + 0.5, (double)z + 0.5, baseBlock->GetBlockId(), data);
        world->AddEntity(entity);
        world->ChangeBlock(x, y, z, 0, 0, false);
    }
}
void BlockFallingScript::InitParam(int paramId, const std::string& param)
{
    switch (paramId)
    {
    case SCRIPTINGPARAM_BLOCK_FALLING_TRAVERSABLEBLOCK:
    {
        std::istringstream traversable_block(param);
        int value;
        for(std::string each; std::getline(traversable_block, each, ',');)
        {
            std::istringstream(each) >> value;
            traversableBlock.insert(value);
        }
        break;
    }
    case SCRIPTINGPARAM_BLOCK_FALLING_TRAVERSABLEMATERIAL:
    {
        std::istringstream traversable_material(param);
        unsigned int value;
        for(std::string each; std::getline(traversable_material, each, ',');)
        {
            std::istringstream(each) >> value;
            if (value > 0 && value <= BLOCK_COUNT)
            {
                Block::Block* block = Block::BlockList::getBlock(value);
                traversableMaterial.insert(block->getMaterial().getId());
            }
        }
        break;
    }
    default:
        std::cerr << "BAD PARAMETER ID: " << paramId << std::endl;
        break;
    }
}

bool BlockFallingScript::isBlockTraversable(World::World* world, int x, i_height y, int z)
{
    i_block bottomBlockId = world->GetBlockId(x, y, z);
    if (bottomBlockId == 0)
    {
        return true;
    }
    else if (traversableBlock.find(bottomBlockId) != traversableBlock.end())
    {
        return true;
    }
    else
    {
        Block::Block* block = Block::BlockList::getBlock(bottomBlockId);
        if (traversableMaterial.find(block->getMaterial().getId()) != traversableMaterial.end())
        {
            return true;
        }
    }
    return false;
}
} /* namespace Scripting */
