#include "BlockFallingScript.h"

#include <cassert>
#include <iostream>

#include "Block/BlockConstants.h"
#include "Entity/EntityPlayer.h"
#include "Entity/Object/EntityFallingBlock.h"
#include "Util/AssertUtil.h"
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

void BlockFallingScript::OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int /*face*/, i_block& blockId, i_data& /*data*/, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    assert(player->getWorld() != NULL);
    player->getWorld()->MarkBlockForUpdate(x, y, z, blockId, 5);
}

void BlockFallingScript::OnNeighborChange(World::World* world, int x, i_height y, int z, i_block /*neighborBlockId*/) const
{
    world->MarkBlockForUpdate(x, y, z, baseBlock->GetBlockId(), 5);
}

void BlockFallingScript::OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const
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
                const Block::Block* block = Block::BlockList::getBlock(value);
                traversableMaterial.insert(block->GetMaterial().getId());
            }
        }
        break;
    }
    default:
        AssertSwitchBadDefault(paramId)
        break;
    }
}

bool BlockFallingScript::isBlockTraversable(World::World* world, int x, i_height y, int z) const
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
        const Block::Block* block = Block::BlockList::getBlock(bottomBlockId);
        if (traversableMaterial.find(block->GetMaterial().getId()) != traversableMaterial.end())
        {
            return true;
        }
    }
    return false;
}
} /* namespace Scripting */
