#include "ScriptManager.h"

#include "Block/Scripts/RegisterBlockScripts.h"
#include "Block/Scripts/BlockScript.h"
#include "Inventory/Scripts/RegisterItemScripts.h"
#include "Inventory/Scripts/ItemScript.h"

#include <iostream>

namespace Scripting
{

ScriptManager* ScriptManager::instance = NULL;

ScriptManager* ScriptManager::GetInstance()
{
    if (instance == NULL)
        instance = new ScriptManager;
    return instance;
}

void ScriptManager::RegisterScript(std::string scriptName, BlockScript* script)
{
    blockScript[scriptName] = script;
}
BlockScript* ScriptManager::GetBlockScript(std::string scriptName)
{
    auto it = blockScript.find(scriptName);
    if (it != blockScript.end())
        return it->second->Copy();
    return NULL;
}

void ScriptManager::RegisterScript(std::string scriptName, ItemScript* script)
{
    itemScript[scriptName] = script;
}
ItemScript* ScriptManager::GetItemScript(std::string scriptName)
{
    auto it = itemScript.find(scriptName);
    if (it != itemScript.end())
        return it->second->Copy();
    return NULL;
}

void ScriptManager::RegisterAllScripts()
{
    RegisterBlockScript();
    RegisterItemScripts();
}

ScriptManager::ScriptManager()
{
}

ScriptManager::~ScriptManager()
{
}

} /* namespace Scripting */
