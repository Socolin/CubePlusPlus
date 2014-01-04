#include "BlockFluidScript.h"


namespace Scripting
{

BlockFluidScript::BlockFluidScript()
    : BlockScript("block_fluid")
{
}

BlockFluidScript::BlockFluidScript(const char* scriptName)
    : BlockScript(scriptName)
{
}

BlockFluidScript::~BlockFluidScript()
{
}

BlockScript* BlockFluidScript::Copy()
{
    return new BlockFluidScript(*this);
}

void BlockFluidScript::OnBlockAdded(World::World* world, int x, i_height y, int z, i_data data) const
{

}

void BlockFluidScript::OnNeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const
{

}


} /* namespace Scripting */
