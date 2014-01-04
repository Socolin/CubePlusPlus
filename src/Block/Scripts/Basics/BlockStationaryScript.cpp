#include "BlockStationaryScript.h"


namespace Scripting
{

BlockStationaryScript::BlockStationaryScript()
    : parent_type("block_stationary")
{
}

BlockStationaryScript::~BlockStationaryScript()
{
}

BlockScript* BlockStationaryScript::Copy()
{
    return new BlockStationaryScript(*this);
}

void BlockStationaryScript::OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const
{

}

void BlockStationaryScript::OnNeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const
{
    parent_type::OnNeighborChange(world, x, y, z, neighborBlockId);
}


} /* namespace Scripting */
