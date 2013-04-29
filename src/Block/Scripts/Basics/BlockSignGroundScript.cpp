#include "BlockSignGroundScript.h"

#include "Block/BlockConstants.h"
#include "Block/TileEntities/TileEntitySign.h"
#include "Entity/EntityPlayer.h"

namespace Scripting
{

BlockSignGroundScript::BlockSignGroundScript()
    : BlockScript("block_signground")
{
}

BlockSignGroundScript::~BlockSignGroundScript()
{
}

BlockScript* BlockSignGroundScript::Copy()
{
    return new BlockSignGroundScript(*this);
}

void BlockSignGroundScript::OnBlockPlacedBy(World::EntityPlayer* player, int /*x*/, i_height /*y*/, int /*z*/, int /*face*/, i_block& /*blockId*/, i_data& data, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    data = (int)(std::floor(((player->getYaw() + 180) * 16.0 / 360.0) + 0.5)) & 15;
}

Block::TileEntity* BlockSignGroundScript::CreateNewTileEntity(int blockX, i_height blockY, int blockZ) const
{
    return new Block::TileEntitySign(blockX, blockY, blockZ);
}

bool BlockSignGroundScript::UseTileEntity() const
{
    return true;
}

} /* namespace Scripting */
