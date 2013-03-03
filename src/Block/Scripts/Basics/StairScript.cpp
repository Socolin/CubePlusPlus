#include "StairScript.h"

#include <cmath>
#include "Entity/EntityPlayer.h"
#include "Block/BlockConstants.h"

namespace Scripting
{

StairScript::StairScript()
    : BlockScript("block_stair")
{
}

StairScript::~StairScript()
{
}

BlockScript* StairScript::Copy()
{
    return new StairScript(*this);
}

void StairScript::OnBlockPlacedBy(World::EntityPlayer* player, int x, unsigned char y, int z, int face, short& blockId, short& data)
{
    int playerRotation = (int)(std::floor((player->getYaw() * 4.0 / 360.0) + 0.5)) & 3;
    data = 0;
    if (face == FACE_BOTTOM)
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

} /* namespace Scripting */
