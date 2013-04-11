#ifndef ITEMPAINTINGSCRIPT_H_
#define ITEMPAINTINGSCRIPT_H_

#include "Inventory/Scripts/ItemScript.h"

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('item_painting',0);
 */
class ItemPaintingScript : public ItemScript
{
public:
    ItemPaintingScript();
    virtual ~ItemPaintingScript();
    virtual ItemScript* Copy() override;
    virtual bool OnUseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ) override;
};

} /* namespace Scripting */
#endif /* ITEMPAINTINGSCRIPT_H_ */
