#include "BlockGrassScript.h"

#include "World/World.h"
#include "Util/AssertUtil.h"

namespace Scripting
{

BlockGrassScript::BlockGrassScript()
    : BlockScript("block_grass")
{
    dirtBlockId = 0;
    grassBlockId = 0;
}

BlockGrassScript::~BlockGrassScript()
{
}

BlockScript* BlockGrassScript::Copy()
{
    return new BlockGrassScript(*this);
}
void BlockGrassScript::OnUpdateTick(World::World* world, int x, i_height y, int z, i_data /*data*/) const
{
    i_lightvalue blockRealLightValue = world->GetRealLightValueAt(x, y + 1, z) ;
    if (blockRealLightValue < 4 && world->GetBlockLightOpacity(x, y + 1, z) > 2)
    {
        world->ChangeBlock(x, y, z, dirtBlockId, 0, false);
    }
    else if (blockRealLightValue >= 9)
    {
        for (int i = 0; i < 4; i++)
        {
            int targetBlockX = x + (rand() % 3) - 1;
            i_height targetBlockY = std::min(254, std::max(0, y + (rand() % 5) - 3));
            int targetBlockZ = z + (rand() % 3) - 1;

            i_block targetBlockId = world->GetBlockId(targetBlockX, targetBlockY, targetBlockZ);
            if (targetBlockId == dirtBlockId)
            {
                if (world->GetBlockLightOpacity(targetBlockX, targetBlockY + 1, targetBlockZ) <= 2
                        && world->GetRealLightValueAt(targetBlockX, targetBlockY + 1, targetBlockZ) >= 4)
                {
                    world->ChangeBlock(targetBlockX, targetBlockY, targetBlockZ, grassBlockId, 0, false);
                }
            }

        }
    }
}

void BlockGrassScript::InitParam(int paramId, int param)
{
    switch (paramId)
    {
    case SCRIPTINGPARAM_BLOCK_GRASS_DIRTID:
        dirtBlockId = param;
        break;
    case SCRIPTINGPARAM_BLOCK_GRASS_GRASSID:
        grassBlockId = param;
        break;
    default:
        AssertSwitchBadDefault(paramId)
        break;
    }
}

} /* namespace Scripting */
