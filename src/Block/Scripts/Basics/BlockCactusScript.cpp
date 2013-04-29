#include "BlockCactusScript.h"

#include "Block/BlockConstants.h"
#include "Block/BlockList.h"
#include "World/World.h"
#include "Inventory/InventoryPlayer.h"

namespace Scripting
{

BlockCactusScript::BlockCactusScript()
    : BlockScript("block_cactus")
{
}

BlockCactusScript::~BlockCactusScript()
{
}

BlockScript* BlockCactusScript::Copy()
{
    return new BlockCactusScript(*this);
}

bool BlockCactusScript::CanPlace(World::World* world, int x, i_height y, int z, char /*face*/) const
{
    if((BlockCactusScript::baseBlock->GetBlockId() == world->GetBlockId(x, y - 1, z) || allowed_to_grow_on.find(world->GetBlockId(x, y - 1, z)) != allowed_to_grow_on.end()) && CheckSideBySideBlocks(world, x, y, z))
    	return true;
    
    return false;
}

void BlockCactusScript::OnUpdateTick(World::World* world, int x, i_height y, int z, i_data /*data*/) const
{
	//If the cactus can grow
	if(y+1 < 255 && world->GetBlockId(x, y + 1, z) == 0)
	{
		//Then check the cactus height block by block
		int height = 1;

		for(int i = 1; i < 3 && y - i > 0; i++)
		{
			if(BlockCactusScript::baseBlock->GetBlockId() == world->GetBlockId(x, y - i, z))
				height++;
		}

		//If the cactus height is inferior to three, then we try to add a cactus block on the top
		if(height < 3)
		{
			if(CheckSideBySideBlocks(world, x, y+1, z))
			{
				world->ChangeBlock(x, y+1, z, BlockCactusScript::baseBlock->GetBlockId(), 0, false);
			} //But if the top of the cactus is in contact with a side by side block, let's pop!
			else
			{
				world->DropItemstackWithRandomDirection(x + 0.5, y + 1.5, z + 0.5, Inventory::ItemStack(BlockCactusScript::baseBlock->GetBlockId(),1,0));
			}
		}
	}
}

void BlockCactusScript::GetBoundingBoxes(int /*x*/, int /*y*/, int /*z*/, i_data /*data*/, std::vector<Util::AABB>& /*bbList*/) const
{
}

void BlockCactusScript::InitParam(int paramId, const std::string& param)
{
    if(paramId == 1)
    {
    	std::istringstream allowed_to_grow_on_list(param);
    	int value;
    	for(std::string each; std::getline(allowed_to_grow_on_list, each, ',');)
    	{
    		std::istringstream(each) >> value;
    		allowed_to_grow_on.insert(value);
    	}
    }
    else
    {
    	std::cerr << "BAD PARAMETER ID: " << paramId << std::endl;
    }
}

void BlockCactusScript::OnNeighborChange(World::World* world, int x, i_height y, int z, i_block /*neighborBlockId*/) const
{
	if(!CheckSideBySideBlocks(world, x, y, z) || (BlockCactusScript::baseBlock->GetBlockId() != world->GetBlockId(x, y - 1, z) && allowed_to_grow_on.find(world->GetBlockId(x, y - 1, z)) == allowed_to_grow_on.end()))
	{
		world->ChangeBlock(x, y, z, 0, 0, false);
		world->DropItemstackWithRandomDirection(x + 0.5, y + 0.5, z + 0.5, Inventory::ItemStack(BlockCactusScript::baseBlock->GetBlockId(),1,0));
	}
}

bool BlockCactusScript::CheckSideBySideBlocks(World::World* world, int x, i_height y, int z) const
{
	//Return false if there is a block in contact with the cactus
	if(world->GetBlockId(x - 1, y, z) != 0 || world->GetBlockId(x + 1, y, z) != 0 || world->GetBlockId(x, y, z - 1) != 0 || world->GetBlockId(x, y, z + 1) != 0)
		return false;

	return true;
}

} /* namespace Scripting */
