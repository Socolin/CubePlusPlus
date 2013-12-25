#include "BlockFireScript.h"


namespace Scripting
{

BlockFireScript::BlockFireScript()
    : BlockScript("block_fire")
{
}

BlockFireScript::~BlockFireScript()
{
}

BlockScript* BlockFireScript::Copy()
{
    return new BlockFireScript(*this);
}

void BlockFireScript::Init(Block::Block* baseBlock)
{

}

void BlockFireScript::InitParam(int paramId, int param)
{

}

bool BlockFireScript::CanPlace(World::World* world, int x, i_height y, int z, char face) const
{

}

void BlockFireScript::OnBlockAdded(World::World* world, int x, i_height y, int z, i_data data) const
{

}

void BlockFireScript::OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const
{

}


} /* namespace Scripting */
