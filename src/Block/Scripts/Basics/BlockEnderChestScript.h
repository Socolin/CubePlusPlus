#ifndef BLOCKENDERCHESTSCRIPT_H_
#define BLOCKENDERCHESTSCRIPT_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

/*
INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_enderchest',1);
SET @scriptId = LAST_INSERT_ID();
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,1,'windowEnderChestChestDataId',1);
*/

#define SCRIPTINGPARAM_BLOCK_ENDERCHEST_WINDOWENDERCHESTCHESTDATAID 1
class BlockEnderChestScript : public BlockScript
{
public:
    BlockEnderChestScript();
    virtual ~BlockEnderChestScript();
    virtual BlockScript* Copy() override;

    virtual void InitParam(int paramId, int param);

    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
    virtual ItemUseResult OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
    virtual Block::TileEntity* CreateNewTileEntity(World::World* world, int blockX, i_height blockY, int blockZ) const override;
    virtual bool UseTileEntity() const override;
private:
    i_windowDataId windowEnderChestChestDataId;
};

} /* namespace Scripting */
#endif /* BLOCKENDERCHESTSCRIPT_H_ */
