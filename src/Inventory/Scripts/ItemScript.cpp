#include "ItemScript.h"

#include "Scripting/ScriptManager.h"

namespace Scripting
{

ItemScript::ItemScript(const char* scriptName)
    : baseItem(0)
{
    ScriptManager::GetInstance()->RegisterScript(scriptName, this);
}

ItemScript::~ItemScript()
{
}

void ItemScript::Init(Inventory::Item* item)
{
    baseItem = item;
}

} /* namespace Scripting */
