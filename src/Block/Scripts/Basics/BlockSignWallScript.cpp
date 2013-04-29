#include "BlockSignWallScript.h"

#include "Block/BlockConstants.h"
#include "Block/TileEntities/TileEntitySign.h"

namespace Scripting
{

BlockSignWallScript::BlockSignWallScript()
    : BlockScript("block_signwall")
{
}

BlockSignWallScript::~BlockSignWallScript()
{
}

BlockScript* BlockSignWallScript::Copy()
{
    return new BlockSignWallScript(*this);
}

void BlockSignWallScript::OnBlockPlacedBy(World::EntityPlayer* /*player*/, int /*x*/, i_height /*y*/, int /*z*/, int face, i_block& /*blockId*/, i_data& data, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    switch (face)
    {
     case FACE_BOTTOM: // -Y
     case FACE_TOP: // +Y
         return; // MUST NOT HAPPEN
     case FACE_NORTH: // -Z
         data = 2;
         break;
     case FACE_SOUTH: // +Z
         data = 3;
         break;
     case FACE_WEST: // -X
         data = 4;
         break;
     case FACE_EAST: // +X
         data = 5;
         break;
     case FACE_NONE:
         return;
     };
}

Block::TileEntity* BlockSignWallScript::CreateNewTileEntity(int blockX, i_height blockY, int blockZ) const
{
    return new Block::TileEntitySign(blockX, blockY, blockZ);
}

bool BlockSignWallScript::UseTileEntity() const
{
    return true;
}

} /* namespace Scripting */
