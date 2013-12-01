#include "Item.h"

#include <iostream>

#include "Entity/EntityPlayer.h"
#include "Inventory/ItemStack.h"
#include "Scripts/ItemScript.h"

namespace Inventory
{
Item::Item(i_item itemId, unsigned char maxStackSize, i_data maxDamage, bool hasSubType, unsigned short containerId, int burningTime, Scripting::ItemScript* script)
    : itemId(itemId)
    , maxStackSize(maxStackSize)
    , maxDamage(maxDamage)
    , hasSubType(hasSubType)
    , containerId(containerId)
    , burningTime(burningTime)
    , script(script)
{
}

Item::~Item()
{
    delete script;
}

ItemUseResult Item::UseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const
{
    if (script != nullptr)
    {
        return script->OnUseOnBlock(user, x, y, z, face, cursorPositionX, cursorPositionY, cursorPositionZ);
    }
    return ItemUseResult{false, false, 0};
}

ItemUseResult Item::Use(World::EntityPlayer* user) const
{
    if (script != nullptr)
    {
        return script->OnUse(user);
    }
    return ItemUseResult{false, false, 0};
}

bool Item::CanHarvestBlock(i_block /*blockId*/) const
{
    // TODO: maybe database data ?
    return false;
}

float Item::GetStrengthVsBlock(i_block /*blockId*/) const
{
    // TODO:
    return 1.f;
}

} /* namespace Inventory */
