#include "Item.h"

#include <iostream>

#include "Entity/EntityPlayer.h"
#include "Inventory/ItemStack.h"
#include "Scripts/ItemScript.h"

namespace Inventory
{
Item::Item(unsigned short itemId, unsigned int maxStackSize, unsigned int maxDamage, bool hasSubType, unsigned short containerId, Scripting::ItemScript* script)
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

bool Item::UseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, ItemStack& item, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const
{
    if (script != nullptr)
    {
        return script->OnUseOnBlock(user, x, y, z, face, item, cursorPositionX, cursorPositionY, cursorPositionZ);
    }
    return false;
}

bool Item::Use(World::EntityPlayer* /*user*/, ItemStack& /*item*/) const
{
	if (script != nullptr)
	{
		// TODO
	}
	return false;
}

} /* namespace Inventory */
