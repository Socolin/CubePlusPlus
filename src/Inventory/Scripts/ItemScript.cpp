#include "ItemScript.h"

#include "Scripting/ScriptManager.h"

namespace Scripting
{

ItemScript::ItemScript(const char* scriptName)
    : baseItem(nullptr)
{
    ScriptManager::Instance().RegisterScript(scriptName, this);
}

ItemScript::~ItemScript()
{
}

void ItemScript::Init(Inventory::Item* item)
{
    baseItem = item;
}

bool ItemScript::OnUse(World::EntityPlayer* /*user*/) const
{
    return false;
}

bool ItemScript::OnUseOnEntity(World::EntityPlayer* /*user*/, World::Entity* /*target*/) const
{
    return false;
}

bool ItemScript::OnUseOnBlock(World::EntityPlayer* /*user*/, int /*x*/, unsigned char /*y*/, int /*z*/, char /*face*/, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    return false;
}

} /* namespace Scripting */
