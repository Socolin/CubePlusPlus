#include "BlockQuartzScript.h"

#include "Block/BlockConstants.h"

namespace Scripting
{

BlockQuartzScript::BlockQuartzScript()
    : BlockScript("block_quartz")
{
}

BlockQuartzScript::~BlockQuartzScript()
{
}

BlockScript* BlockQuartzScript::Copy()
{
    return new BlockQuartzScript(*this);
}

void BlockQuartzScript::OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const
{
    if (data == 2)
    {
        switch (face)
        {
         case FACE_BOTTOM: // -Y
         case FACE_TOP: // +Y
             break;
         case FACE_NORTH: // -Z
         case FACE_SOUTH: // +Z
             data = 4;
             break;
         case FACE_WEST: // -X
         case FACE_EAST: // +X
             data = 3;
             break;
         case FACE_NONE:
             return;
         };
    }
}
} /* namespace Scripting */
