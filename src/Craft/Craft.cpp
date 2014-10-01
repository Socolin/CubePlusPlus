#include "Craft.h"

#include "Inventory/InventoryCraft.h"
#include "Scripts/CraftScript.h"

namespace Craft
{

Craft::Craft(char width, char height, i_item resultId, i_damage resultData, int resultQtt, Scripting::CraftScript* script)
    : width(width)
    , height(height)
    , script(script)
{
    result = new Inventory::ItemStack(resultId, resultQtt, resultData);
}

Craft::~Craft()
{
    delete result;
    delete script;
}

const Inventory::ItemStack* Craft::GetResult() const
{
    if (script != nullptr)
    {
        return result;//TODO
    }
    return result;
}

bool Craft::Match(Inventory::InventoryCraft* craftInventory) const
{
    if (script != nullptr)
    {
        return script->Match(craftInventory);
    }
    return false;
}

void Craft::SetNextSlot(i_item itemId, i_damage itemData)
{
    if (script != nullptr)
    {
        script->SetNextSlot(itemId, itemData);
    }
}

char Craft::GetHeight() const
{
    return height;
}

char Craft::GetWidth() const
{
    return width;
}

} /* namespace Craft */
