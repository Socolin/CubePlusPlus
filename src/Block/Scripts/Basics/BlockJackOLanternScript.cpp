#include "BlockJackOLanternScript.h"

#include <cmath>
#include "Entity/EntityPlayer.h"
#include "Block/BlockConstants.h"

namespace Scripting
{

BlockJackOLanternScript::BlockJackOLanternScript() :
         BlockScript("block_jackolantern")
{
}

BlockJackOLanternScript::~BlockJackOLanternScript()
{
}

BlockScript* BlockJackOLanternScript::Copy()
{
    return new BlockJackOLanternScript(*this);
}

void BlockJackOLanternScript::OnBlockPlacedBy(World::EntityPlayer* player, int /*x*/, i_height /*y*/, int /*z*/, int /*face*/, i_block& /*blockId*/, i_data& data, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    int playerRotation = (int)(std::floor((player->getYaw() * 4.0 / 360.0) + 0.5)) & 3;
    data = 0;

    switch (playerRotation)
    {
    case 0:
        data |= 2;
        break;
    case 1:
        data |= 3;
        break;
    case 2:
        data |= 0;
        break;
    case 3:
        data |= 1;
        break;
    }
}

} /* namespace Scripting */

