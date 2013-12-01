#include "BlockScript.h"

#include <iostream>
#include "Scripting/ScriptManager.h"
#include "Entity/EntityPlayer.h"

namespace Scripting
{

BlockScript::BlockScript(const char* scriptName)
    : baseBlock(nullptr)
{
    ScriptManager::Instance().RegisterScript(scriptName, this);
}

BlockScript::~BlockScript()
{

}

void BlockScript::Init(Block::Block* baseBlock)
{
    this->baseBlock = baseBlock;
}

Block::TileEntity* BlockScript::CreateNewTileEntity(World::World* /*world*/, int /*blockX*/, i_height /*blockY*/, int /*blockZ*/) const
{
    return nullptr;
}

bool BlockScript::UseTileEntity() const
{
    return false;
}

void BlockScript::OnUpdateTick(World::World* /*world*/, int /*x*/, i_height /*y*/, int /*z*/, i_data /*data*/) const
{
}

void BlockScript::OnNeighborChange(World::World* /*world*/, int /*x*/, i_height /*y*/, int /*z*/, i_block /*neighborBlockId*/) const
{
}

void BlockScript::GetBoundingBoxes(int x, int y, int z, i_data /*data*/, std::vector<Util::AABB>& bbList) const
{
    bbList.push_back(baseBlock->GetBoundingBox(x, y, z));
}

void BlockScript::OnBlockPlacedBy(World::EntityPlayer* /*player*/, int /*x*/, i_height /*y*/, int /*z*/, int /*face*/, i_block& /*blockId*/, i_data& /*data*/, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
}

ItemUseResult BlockScript::OnUseBlock(World::EntityPlayer* /*user*/, int /*x*/, i_height /*y*/, int /*z*/, char /*face*/, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    return ItemUseResult{false, false, 0};
}

bool BlockScript::CanPlace(World::World* /*world*/, int /*x*/, i_height /*y*/, int /*z*/, char /*face*/) const
{
    return true;
}

void BlockScript::OnDestroy(World::World* /*world*/, int /*x*/, i_height /*y*/, int /*z*/, i_data /*data*/) const
{
}

bool BlockScript::CanProvidePower() const
{
    return false;
}

i_powerlevel BlockScript::GetWeakPowerLevel(World::World* /*world*/, int /*x*/, i_height /*y*/, int /*z*/, int /*side*/, i_data /*metadata*/) const
{
    return 0;
}

void BlockScript::OnBlockAdded(World::World* /*world*/, int /*x*/, i_height /*y*/, int /*z*/, i_data /*data*/) const
{
}

bool BlockScript::HasSolidTopSurface(i_data /*metadata*/) const
{
    return !baseBlock->GetMaterial().isTranslucent() && baseBlock->IsRenderAsNormal();
}

i_powerlevel BlockScript::GetStrongPowerLevel(World::World* /*world*/, int /*x*/, i_height /*y*/, int /*z*/, int /*side*/, i_data /*metadata*/) const
{
    return 0;
}

bool BlockScript::IsFullBlock() const
{
    return baseBlock->IsEdgeAverageMoreThanOne();
}

void BlockScript::OnNotifyTileEntityStateChange(World::World* /*world*/, int /*x*/, i_height /*y*/, int /*z*/, int /*action*/)
{
}

} /* namespace Scripting */
