#ifndef ITEMBLOCKSCRIPT_H_
#define ITEMBLOCKSCRIPT_H_

#include "Inventory/Scripts/ItemScript.h"

namespace Scripting
{

class ItemBlockScript : public ItemScript
{
public:
    ItemBlockScript();
    virtual ~ItemBlockScript();
    virtual ItemScript* Copy();
    virtual bool OnUseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, Inventory::ItemStack* item);
};

} /* namespace Scripting */
#endif /* ITEMBLOCKSCRIPT_H_ */
