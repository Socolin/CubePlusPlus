#include "ScriptManager.h"

#include "Block/Scripts/RegisterBlockScripts.h"
#include "Block/Scripts/BlockScript.h"
#include "Craft/Scripts/RegisterCraftScripts.h"
#include "Craft/Scripts/CraftScript.h"
#include "Inventory/Scripts/RegisterItemScripts.h"
#include "Inventory/Scripts/ItemScript.h"
#include "Database/DatabaseManager.h"
#include "Window/Scripts/RegisterWindowScript.h"
#include "Window/Scripts/WindowScript.h"


#include <iostream>

namespace Scripting
{
void ScriptManager::RegisterScript(std::string scriptName, BlockScript* script)
{
    if (blockScript.find(scriptName) == blockScript.end())
    {
        std::cout << "\t- " << scriptName << std::endl;
        blockScript[scriptName] = script;
    }
    else
    {
        assert(false);
    }
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
    if (itemScript.find(scriptName) == itemScript.end())
    {
        std::cout << "\t- " << scriptName << std::endl;
        itemScript[scriptName] = script;
    }
    else
    {
        assert(false);
    }
}
ItemScript* ScriptManager::GetItemScript(std::string scriptName)
{
    auto it = itemScript.find(scriptName);
    if (it != itemScript.end())
        return it->second->Copy();
    return NULL;
}

void ScriptManager::RegisterScript(std::string scriptName, WindowScript* script)
{
    if (windowScript.find(scriptName) == windowScript.end())
    {
        std::cout << "\t- " << scriptName << std::endl;
        windowScript[scriptName] = script;
    }
    else
    {
        assert(false);
    }
}
WindowScript* ScriptManager::GetWindowScript(std::string scriptName)
{
    auto it = windowScript.find(scriptName);
    if (it != windowScript.end())
        return it->second->Copy();
    return NULL;
}

void ScriptManager::RegisterScript(std::string scriptName, CraftScript* script)
{
    if (craftScript.find(scriptName) == craftScript.end())
    {
        std::cout << "\t- " << scriptName << std::endl;
        craftScript[scriptName] = script;
    }
    else
    {
        assert(false);
    }
}

CraftScript* ScriptManager::GetCraftScript(std::string scriptName)
{
    auto it = craftScript.find(scriptName);
    if (it != craftScript.end())
        return it->second->Copy();
    return NULL;
}


void ScriptManager::RegisterAllScripts()
{
    std::cout << "Registering block's scripts: " << std::endl;
    RegisterBlockScript();
    std::cout << "Registering item's scripts: " << std::endl;
    RegisterItemScripts();
    std::cout << "Registering window's scripts: " << std::endl;
    RegisterWindowScript();
    std::cout << "Registering craft's scripts: " << std::endl;
    RegisterCraftScript();
}

void ScriptManager::LoadScriptsIds()
{
    Database::DatabaseManager* db = Database::DatabaseManager::Instance();
    db->connect();

    sql::ResultSet* result = db->querry("SELECT * FROM `script` ORDER BY `id`");

    if (result == nullptr)
    {
        std::cerr << "ERROR: no scripts found in database";
        return;
    }

    std::cout << "Loading scripts's datas" << std::endl;

    while (result->next())
    {
        std::string scriptName = result->getString(TableScript::scriptName);
        uint32_t scriptId = result->getUInt(TableScript::id);

        scriptsIds[scriptId] = scriptName;
    }
    std::cout << scriptsIds.size() << " scripts loaded" << std::endl;

    delete result;
}

std::string ScriptManager::GetScriptName(int scriptId)
{
    return scriptsIds[scriptId];
}

ScriptManager::ScriptManager()
{
    LoadScriptsIds();
}

ScriptManager::~ScriptManager()
{
    for (auto itr : blockScript)
    {
        delete itr.second;
    }
    blockScript.clear();

    for (auto itr : itemScript)
    {
        delete itr.second;
    }
    itemScript.clear();

    for (auto itr : windowScript)
    {
        delete itr.second;
    }
    windowScript.clear();

    for (auto itr : craftScript)
    {
        delete itr.second;
    }
    craftScript.clear();
}

} /* namespace Scripting */
