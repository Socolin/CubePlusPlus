#ifndef ITEMBLOCKSCRIPT_H_
#define ITEMBLOCKSCRIPT_H_

#include "Inventory/Scripts/ItemScript.h"

/*
 *
INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('item_block',2);
SET @scriptId = LAST_INSERT_ID();
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,1,'blockid',1);
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,2,'UseMetadata',1);
 */
#define SCRIPTINGPARAM_ITEM_BLOCK_BLOCKID 1
#define SCRIPTINGPARAM_ITEM_BLOCK_USEMETADATA 2
namespace Scripting
{

class ItemBlockScript : public ItemScript
{
public:
    ItemBlockScript();
    virtual ~ItemBlockScript();
    virtual ItemScript* Copy();
    virtual bool OnUseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ);
    virtual void InitParam(int paramId, int param);
private:
    i_block AssociatedBlockId;
    bool UseMetadata;
};

} /* namespace Scripting */
#endif /* ITEMBLOCKSCRIPT_H_ */
