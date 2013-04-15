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

Block::TileEntity* BlockScript::CreateNewTileEntity(int blockX, i_height blockY, int blockZ) const
{
    return nullptr;
}

bool BlockScript::UseTileEntity() const
{
    return false;
}

void BlockScript::OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const
{
}

void BlockScript::OnNeighborChange(World::World* world, int x, i_height y, int z) const
{
}

void BlockScript::GetBoundingBoxes(int x, int y, int z, i_data data, std::vector<Util::AABB>& bbList) const
{
    bbList.push_back(baseBlock->GetBoundingBox(x, y, z));
}

void BlockScript::OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const
{
}

bool BlockScript::OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const
{
    return false;
}

bool BlockScript::CanPlace(World::World* world, int x, i_height y, int z, char face) const
{
    return true;
}

void BlockScript::OnDestroy(World::World* world, int x, i_height y, int z, i_data data) const
{
}

} /* namespace Scripting */
