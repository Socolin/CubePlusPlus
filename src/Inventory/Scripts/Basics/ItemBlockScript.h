#ifndef ITEMBLOCKSCRIPT_H_
#define ITEMBLOCKSCRIPT_H_

#include "Inventory/Scripts/ItemScript.h"

/*
 *
INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('item_block',1);
SET @scriptId = LAST_INSERT_ID();
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,1,'blockid',1);
 */
#define SCRIPTINGPARAM_ITEM_BLOCK_BLOCKID 1
namespace Scripting
{

class ItemBlockScript : public ItemScript
{
public:
    ItemBlockScript();
    virtual ~ItemBlockScript();
    virtual ItemScript* Copy();
    virtual bool OnUseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, Inventory::ItemStack& item);
    virtual void InitParam(int paramId, int param);
private:
    int AssociatedBlockId;
};

} /* namespace Scripting */
#endif /* ITEMBLOCKSCRIPT_H_ */
