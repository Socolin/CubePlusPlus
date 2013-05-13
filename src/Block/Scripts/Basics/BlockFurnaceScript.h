#ifndef BLOCKFURNACESCRIPT_H_
#define BLOCKFURNACESCRIPT_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_furnace',1);
   SET @scriptId = LAST_INSERT_ID();
   INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,1,'windowId',1);
 */

#define SCRIPTINGPARAM_BLOCK_FURNACe_WINDOWID 1
class BlockFurnaceScript : public BlockScript
{
public:
    BlockFurnaceScript();
    virtual ~BlockFurnaceScript();
    virtual BlockScript* Copy() override;

    virtual void InitParam(int paramId, int param);

    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const;
    virtual bool OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const;
    virtual Block::TileEntity* CreateNewTileEntity(int blockX, i_height blockY, int blockZ) const override;
    virtual bool UseTileEntity() const override;
    virtual void OnDestroy(World::World* world, int x, i_height y, int z, i_data data) const override;
private:
    i_windowDataId windowDataId;
};

} /* namespace Scripting */
#endif /* BLOCKFURNACESCRIPT_H_ */
