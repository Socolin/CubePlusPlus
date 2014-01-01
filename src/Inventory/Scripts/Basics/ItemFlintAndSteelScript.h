#ifndef ITEM_FLINT_AND_STEEL_SCRIPT_H_
#define ITEM_FLINT_AND_STEEL_SCRIPT_H_

#include "Inventory/Scripts/ItemScript.h"

namespace Scripting
{

class ItemFlintAndSteelScript: public ItemScript
{
public:
    ItemFlintAndSteelScript();
    virtual ~ItemFlintAndSteelScript ();
    virtual ItemScript* Copy() override;
    virtual ItemUseResult OnUseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
};
 
} /* namespace Scripting */
#endif /* ITEM_FLINT_AND_STEEL_SCRIPT_H_ */
