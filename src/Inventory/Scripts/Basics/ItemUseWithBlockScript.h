#ifndef ITEMUSEWITHBLOCKSCRIPT_H_
#define ITEMUSEWITHBLOCKSCRIPT_H_

#include "Inventory/Scripts/ItemScript.h"

/*
 *
INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('item_usewithblock',2);
SET @scriptId = LAST_INSERT_ID();
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,2,'UseMetadata',1);
 */
//#define SCRIPTINGPARAM_ITEM_BLOCK_BLOCKID 1
//#define SCRIPTINGPARAM_ITEM_BLOCK_USEMETADATA 2
namespace Scripting
{

class ItemUseWithBlockScript : public ItemScript
{
public:
	ItemUseWithBlockScript();
    virtual ~ItemUseWithBlockScript();
    virtual ItemScript* Copy() override;
    virtual bool OnUseOnBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ) override;
    virtual void InitParam(int paramId, int param) override;
private:
    bool UseMetadata;
};

} /* namespace Scripting */
#endif /* ITEMUSEWITHBLOCKSCRIPT_H_ */
