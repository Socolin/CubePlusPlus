#ifndef BLOCKSIGNWALLSCRIPT_H_
#define BLOCKSIGNWALLSCRIPT_H_

#include "Block/Scripts/BlockScript.h"
namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_signwall',0);
 */
class BlockSignWallScript : public BlockScript
{
public:
    BlockSignWallScript();
    virtual ~BlockSignWallScript();
    virtual BlockScript* Copy() override;
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
    virtual Block::TileEntity* CreateNewTileEntity(int blockX, i_height blockY, int blockZ) const;
    virtual bool UseTileEntity() const;
};

} /* namespace Scripting */
#endif /* BLOCKSIGNWALLSCRIPT_H_ */
