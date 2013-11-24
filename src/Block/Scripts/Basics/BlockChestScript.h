#ifndef BLOCKCHESTSCRIPT_H_
#define BLOCKCHESTSCRIPT_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

/*
   INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_chest',2);
   SET @scriptId = LAST_INSERT_ID();
   INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,1,'windowChestDataId',1);
   INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,2,'windowDoubleChestDataId',1);
 */

#define SCRIPTINGPARAM_BLOCK_CHEST_CHESTWINDOWID 1
#define SCRIPTINGPARAM_BLOCK_CHEST_DOUBLECHESTWINDOWID 2

class BlockChestScript : public BlockScript
{
public:
    BlockChestScript();
    virtual ~BlockChestScript();
    virtual BlockScript* Copy() override;

    virtual void InitParam(int paramId, int param);

    virtual bool CanPlace(World::World* world, int x, i_height y, int z, char face) const override;
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
    virtual World::ItemUseResult OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
    virtual Block::TileEntity* CreateNewTileEntity(World::World* world, int blockX, i_height blockY, int blockZ) const override;
    virtual bool UseTileEntity() const override;
    virtual void OnDestroy(World::World* world, int x, i_height y, int z, i_data data) const override;
private:
    i_windowDataId windowChestDataId;
    i_windowDataId windowDoubleChestDataId;
};

} /* namespace Scripting */
#endif /* BLOCKCHESTSCRIPT_H_ */
