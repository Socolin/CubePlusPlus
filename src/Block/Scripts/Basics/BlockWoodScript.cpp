#include "BlockWoodScript.h"
#include "Block/BlockConstants.h"

namespace Scripting
{

BlockWoodScript::BlockWoodScript()
    : BlockScript("block_wood")
{
}

BlockWoodScript::~BlockWoodScript()
{
}

BlockScript* BlockWoodScript::Copy()
{
    return new BlockWoodScript(*this);
}

void BlockWoodScript::OnBlockPlacedBy(World::EntityPlayer* /*player*/, int /*x*/, i_height /*y*/, int /*z*/, int face, i_block& /*blockId*/, i_data& data, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    switch (face)
    {
     case FACE_BOTTOM: // -Y
     case FACE_TOP: // +Y
         break;
     case FACE_NORTH: // -Z
     case FACE_SOUTH: // +Z
         data |= 0x8;
         break;
     case FACE_WEST: // -X
     case FACE_EAST: // +X
         data |= 0x4;
         break;
     case FACE_NONE:
         return;
     };
}

} /* namespace Scripting */
