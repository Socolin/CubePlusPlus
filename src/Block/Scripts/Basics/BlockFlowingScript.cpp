#include "BlockFlowingScript.h"


namespace Scripting
{

BlockFlowingScript::BlockFlowingScript()
    : parent_type("block_flowing")
{
}

BlockFlowingScript::~BlockFlowingScript()
{
}

BlockScript* BlockFlowingScript::Copy()
{
    return new BlockFlowingScript(*this);
}

void BlockFlowingScript::OnBlockAdded(World::World* world, int x, i_height y, int z, i_data data) const
{
    parent_type::OnBlockAdded(world, x, y, z, data);
}

void BlockFlowingScript::OnUpdateTick(World::World* /*world*/, int /*x*/, i_height /*y*/, int /*z*/, i_data /*data*/) const
{

}


} /* namespace Scripting */
