#ifndef BLOCKJUKEBOXSCRIPT_H_
#define BLOCKJUKEBOXSCRIPT_H_

#include "Block/Scripts/BlockScript.h"
#include <set>

#define PARAM_BLOCK_JUKEBOX_ALLOWED_ITEMS 1

namespace Scripting
{

/*
INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_jukebox',1);
SET @scriptId = LAST_INSERT_ID();
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,1,'item_list',0);
 */
class BlockJukeboxScript : public BlockScript
{
public:
    BlockJukeboxScript();
    virtual ~BlockJukeboxScript();
    virtual BlockScript* Copy() override;
    virtual bool OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
    virtual Block::TileEntity* CreateNewTileEntity() const override;
    virtual bool UseTileEntity() const override;
    virtual void OnDestroy(World::World* world, int x, i_height y, int z, i_data data) const override;
    virtual void InitParam(int paramId, const std::string& param) override;
private:
    void EjectRecord(World::World* world, int x, i_height y, int z) const;
    std::set<i_block> item_list;
};

} /* namespace Scripting */
#endif /* BLOCKJUKEBOXSCRIPT_H_ */
