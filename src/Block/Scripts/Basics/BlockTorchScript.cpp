#include "BlockTorchScript.h"

#include "Block/BlockConstants.h"
#include "Block/BlockList.h"
#include "World/World.h"

namespace Scripting
{

BlockTorchScript::BlockTorchScript()
    : BlockScript("block_torch")
{
}

BlockTorchScript::~BlockTorchScript()
{
}

BlockTorchScript::BlockTorchScript(const char* scriptName)
    : BlockScript(scriptName)
{
}

BlockScript* BlockTorchScript::Copy()
{
    return new BlockTorchScript(*this);
}

bool BlockTorchScript::CanPlace(World::World* world, int x, unsigned char y, int z, char face) const
{
    switch (face)
    {
        case FACE_NORTH: // -Z
            z++;
            break;
        case FACE_SOUTH: // +Z
            z--;
            break;
        case FACE_WEST: // -X
            x++;
            break;
        case FACE_EAST: // +X
            x--;
            break;
        default:
        	y--;
        	break;
    };

    const Block::Block* clickedBlock = Block::BlockList::getBlock(world->GetBlockId(x, y, z));
    
    if(clickedBlock != NULL)
    {
    	if(clickedBlock->IsOpaqueCube())
    	{
    		return true;
    	}
    }
    
    return false;
}

void BlockTorchScript::OnBlockPlacedBy(World::EntityPlayer* /*player*/, int /*x*/, i_height /*y*/, int /*z*/, int face, i_block& /*blockId*/, i_data& data, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
	switch (face)
    {
     case FACE_NORTH: // -Z
         data = 4;
         break;
     case FACE_SOUTH: // +Z
         data = 3;
         break;
     case FACE_WEST: // -X
         data = 2;
         break;
     case FACE_EAST: // +X
         data = 1;
         break;
     case FACE_NONE:
     default:
         data = 5;
         return;
     };
}

void BlockTorchScript::GetBoundingBoxes(int /*x*/, int /*y*/, int /*z*/, i_data /*data*/, std::vector<Util::AABB>& /*bbList*/) const
{
}

} /* namespace Scripting */
