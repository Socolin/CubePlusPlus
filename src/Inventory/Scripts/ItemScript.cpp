#include "ItemScript.h"

#include "Scripting/ScriptManager.h"
#include "Entity/EntityPlayer.h"

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

ItemUseResult ItemScript::OnUse(World::EntityPlayer* /*user*/) const
{
    return ItemUseResult{false, false, 0};
}

ItemUseResult ItemScript::OnUseOnEntity(World::EntityPlayer* /*user*/, World::Entity* /*target*/) const
{
    return ItemUseResult{false, false, 0};
}

ItemUseResult ItemScript::OnUseOnBlock(World::EntityPlayer* /*user*/, int /*x*/, unsigned char /*y*/, int /*z*/, char /*face*/, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    return ItemUseResult{false, false, 0};
}

} /* namespace Scripting */
