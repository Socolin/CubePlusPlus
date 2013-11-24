#include "CraftScript.h"

#include "Scripting/ScriptManager.h"

namespace Scripting
{

CraftScript::CraftScript(const char* scriptName)
    : baseCraft(nullptr)
{
    ScriptManager::Instance().RegisterScript(scriptName, this);
}

CraftScript::~CraftScript()
{
}

void CraftScript::SetNextSlot(i_item /*itemId*/, i_damage /*itemData*/)
{
}

void CraftScript::Init(Craft::Craft* baseCraft)
{
    this->baseCraft = baseCraft;
}

bool CraftScript::Match(Inventory::InventoryCraft* /*craftInventory*/) const
{
    return false;
}

} /* namespace Scripting */
