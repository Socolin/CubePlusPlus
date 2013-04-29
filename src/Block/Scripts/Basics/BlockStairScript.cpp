#include "BlockStairScript.h"

#include <cmath>
#include "Entity/EntityPlayer.h"
#include "Block/BlockConstants.h"

namespace Scripting
{

BlockStairScript::BlockStairScript()
        : BlockScript("block_stair")
{
}

BlockStairScript::~BlockStairScript()
{
}

BlockScript* BlockStairScript::Copy()
{
    return new BlockStairScript(*this);
}

void BlockStairScript::OnBlockPlacedBy(World::EntityPlayer* player, int /*x*/, i_height /*y*/, int /*z*/, int face, i_block& /*blockId*/, i_data& data, char /*cursorPositionX*/, char cursorPositionY, char /*cursorPositionZ*/) const
{
    int playerRotation = (int)(std::floor((player->getYaw() * 4.0 / 360.0) + 0.5)) & 3;
    data = 0;
    if (face == FACE_BOTTOM)
        data = 4;

    if (face != FACE_TOP && cursorPositionY >= 8)
        data = 4;

    switch (playerRotation)
    {
    case 0:
        data |= 2;
        break;
    case 1:
        data |= 1;
        break;
    case 2:
        data |= 3;
        break;
    case 3:
        data |= 0;
        break;
    }
}

bool BlockStairScript::HasSolidTopSurface(i_data metadata) const
{
    return (metadata & 0x4);
}

} /* namespace Scripting */
