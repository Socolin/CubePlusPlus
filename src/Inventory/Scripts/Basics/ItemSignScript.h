#ifndef ITEMSIGNSCRIPT_H_
#define ITEMSIGNSCRIPT_H_

#include "Inventory/Scripts/ItemScript.h"

/*
 *
INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('item_sign',2);
SET @scriptId = LAST_INSERT_ID();
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,1,'groundBlockId',1);
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,2,'wallBlockId',1);
 */
#define SCRIPTINGPARAM_ITEM_SIGN_GROUNDBLOCKID 1
#define SCRIPTINGPARAM_ITEM_SIGN_WALLBLOCKID 2
namespace Scripting
{

class ItemSignScript : public ItemScript
{
public:
    ItemSignScript();
    virtual ~ItemSignScript();
    virtual ItemScript* Copy() override;
    virtual bool OnUseOnBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
    virtual void InitParam(int paramId, int param) override;
private:
    i_block groundBlockId;
    i_block wallBlockId;
};

} /* namespace Scripting */
#endif /* ITEMSIGNSCRIPT_H_ */
