#include "BlockRedstonePowered.h"

namespace Scripting
{

BlockRedstonePowered::BlockRedstonePowered()
    : BlockScript("block_redstonepowered")
{
}

BlockRedstonePowered::~BlockRedstonePowered()
{
}

BlockScript* BlockRedstonePowered::Copy()
{
    return new BlockRedstonePowered(*this);
}

bool BlockRedstonePowered::CanProvidePower() const
{
    return true;
}

i_powerlevel BlockRedstonePowered::GetWeakPowerLevel(World::World* /*world*/, int /*x*/, i_height /*y*/, int /*z*/, int /*side*/, i_data /*metadata*/) const
{
    return 15;
}

} /* namespace Scripting */
