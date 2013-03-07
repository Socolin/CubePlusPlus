#include "Item.h"

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

bool Item::UseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
{
    if (script != nullptr)
    {
        return script->OnUseOnBlock(user, x, y, z, face, item, CursorpositionX, CursorpositionY, CursorpositionZ);
    }
    return false;
}

bool Item::Use(World::EntityPlayer* user, ItemStack& item)
{
	if (script != nullptr)
	{
		// TODO
	}
	return false;
}

} /* namespace Scripting */
