#ifndef ITEMFRAMESCRIPT_H_
#define ITEMFRAMESCRIPT_H_

#include "Inventory/Scripts/ItemScript.h"

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('item_frame',0);
 */
class ItemFrameScript : public ItemScript
{
public:
    ItemFrameScript();
    virtual ~ItemFrameScript();
    virtual ItemScript* Copy() override;
    virtual bool OnUseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ) override;
};

} /* namespace World */
#endif /* ITEMFRAMESCRIPT_H_ */
