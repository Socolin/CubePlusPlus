#include "BlockStationaryScript.h"
#include "Util/AssertUtil.h"
#include "World/World.h"


namespace Scripting
{

BlockStationaryScript::BlockStationaryScript()
    : parent_type("block_stationary")
    , tickRate(0)
{
}

BlockStationaryScript::BlockStationaryScript(const char* scriptName)
    : parent_type(scriptName)
    , tickRate(0)
{
}

BlockStationaryScript::~BlockStationaryScript()
{
}

BlockScript* BlockStationaryScript::Copy()
{
    return new BlockStationaryScript(*this);
}

void BlockStationaryScript::InitParam(int paramId, int param)
{
    switch(paramId)
    {
    case SCRIPTINGPARAM_BLOCK_STATIONARY_TICK_RATE:
        tickRate = param;
        break;
    default:
        AssertSwitchBadDefault(paramId);
        break;
    }
}

void BlockStationaryScript::OnNeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const
{
    parent_type::OnNeighborChange(world, x, y, z, neighborBlockId);
    if(neighborBlockId == baseBlock->GetBlockId())
    {
        setNotStationary(world, x, y, z);
    }
}

void BlockStationaryScript::setNotStationary(World::World* world, int x, i_height y, int z) const
{
    s_block_data blockIdAndData = world->GetBlockIdAndData(x, y - 1, z);
    world->ChangeBlock(x, y, z, baseBlock->GetBlockId() - 1, blockIdAndData.blockData);
    world->MarkBlockForUpdate(x, y, z, baseBlock->GetBlockId(), tickRate);
}

} /* namespace Scripting */
