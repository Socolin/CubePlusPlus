#include "ItemBlockScript.h"

namespace Scripting
{

ItemBlockScript::ItemBlockScript()
    : ItemScript("item_block")
{
}

ItemBlockScript::~ItemBlockScript()
{

}

ItemScript* ItemBlockScript::Copy()
{
    return new ItemBlockScript(*this);
}

bool ItemBlockScript::OnUseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, Inventory::ItemStack* item)
{
    return true;
}

} /* namespace Scripting */
