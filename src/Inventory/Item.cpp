#include "Item.h"

#include <iostream>

#include "Entity/EntityPlayer.h"
#include "Inventory/ItemStack.h"
#include "Scripts/ItemScript.h"

namespace Inventory
{
Item::Item(i_item itemId, unsigned char maxStackSize, i_data maxDamage, bool hasSubType, unsigned short containerId, Scripting::ItemScript* script)
    : itemId(itemId)
    , maxStackSize(maxStackSize)
    , maxDamage(maxDamage)
    , hasSubType(hasSubType)
    , containerId(containerId)
    , script(script)
{
}

Item::~Item()
{
}

bool Item::UseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const
{
    if (script != nullptr)
    {
        return script->OnUseOnBlock(user, x, y, z, face, cursorPositionX, cursorPositionY, cursorPositionZ);
    }
    return false;
}

bool Item::Use(World::EntityPlayer* /*user*/) const
{
    if (script != nullptr)
    {
        // TODO
    }
    return false;
}

} /* namespace Inventory */
