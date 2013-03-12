#include "BlockScript.h"

#include <iostream>
#include "Scripting/ScriptManager.h"

namespace Scripting
{

BlockScript::BlockScript(const char* scriptName)
    : baseBlock(NULL)
{
    ScriptManager::GetInstance()->RegisterScript(scriptName, this);
}

BlockScript::~BlockScript()
{

}

void BlockScript::Init(Block::Block* baseBlock)
{
    this->baseBlock = baseBlock;
}

void BlockScript::OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data)
{
}

} /* namespace Inventory */
