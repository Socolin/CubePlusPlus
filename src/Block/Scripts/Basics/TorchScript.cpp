#include "TorchScript.h"
#include "Block/BlockConstants.h"

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
    return false;
}

void TorchScript::OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
{
    switch (face)
    {
     case FACE_BOTTOM: // -Y
     case FACE_TOP: // +Y
         data |= 5;
         break;
     case FACE_NORTH: // -Z
         data |= 4;
         break;
     case FACE_SOUTH: // +Z
         data |= 3;
         break;
     case FACE_WEST: // -X
         data |= 2;
         break;
     case FACE_EAST: // +X
         data |= 1;
         break;
     case FACE_NONE:
         return;
     };
}

} /* namespace Scripting */
