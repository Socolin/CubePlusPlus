#include "BlockRedstoneTorch.h"

#include "World/World.h"

namespace Scripting
{

BlockRedstoneTorch::BlockRedstoneTorch(const char* scriptName)
    : parent_type(scriptName)
{
}

bool BlockRedstoneTorch::isIndirectlyPowered(World::World* world, int x, int y, int z, i_data data) const
{
    // Not usig return a && b && c... for keep this code readable
    if (data == 5 && world->isBlockIndirectlyProvidingPowerTo(x, y - 1, z, 0))
        return true;
    if (data == 3 && world->isBlockIndirectlyProvidingPowerTo(x, y, z - 1, 2))
        return true;
    if (data == 4 && world->isBlockIndirectlyProvidingPowerTo(x, y, z + 1, 3))
        return true;
    if (data == 1 && world->isBlockIndirectlyProvidingPowerTo(x - 1, y, z, 4))
        return true;
    if (data == 2 && world->isBlockIndirectlyProvidingPowerTo(x + 1, y, z, 5))
        return true;
    return false;
}

bool BlockRedstoneTorch::CanProvidePower() const
{
    return true;
}

} /* namespace Scripting */
