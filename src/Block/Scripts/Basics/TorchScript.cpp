#include "TorchScript.h"
#include "Block/BlockConstants.h"
#include "Block/BlockList.h"
#include "World/World.h"

namespace Scripting
{

TorchScript::TorchScript()
    : BlockScript("block_torch")
{
}

TorchScript::~TorchScript()
{
}

BlockScript* TorchScript::Copy()
{
    return new TorchScript(*this);
}

bool TorchScript::CanPlace(World::World* world, int x, unsigned char y, int z, char face)
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

    Block::Block* clickedBlock = Block::BlockList::Instance()->blocks[world->GetBlockId(x, y, z)];
    
    if(clickedBlock != NULL)
    {
    	if(clickedBlock->GetIsOpaqueCube())
    	{
    		return true;
    	}
    }
    
    return false;
}

void TorchScript::OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
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
         data = 0;
         return;
     };
}

void TorchScript::GetBoundingBoxes(int x, int y, int z, i_data data, std::vector<Util::AABB>& bbList)
{
}

} /* namespace Scripting */
