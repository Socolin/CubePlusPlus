#include "ScriptManager.h"

#include "Block/Scripts/RegisterBlockScripts.h"
#include "Block/Scripts/BlockScript.h"
#include "Inventory/Scripts/RegisterItemScripts.h"
#include "Inventory/Scripts/ItemScript.h"
#include "Database/DatabaseManager.h"


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
    std::cout << scriptName << " script registerd";
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
    std::cout << "Start registering scripts";
    RegisterBlockScript();
    RegisterItemScripts();
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
        //uint32_t paramCount = result->getUInt(TableScript::paramCount);

        scriptsIds[scriptId] = scriptName;
    }
    std::cout << scriptsIds.size() << " scripts loaded" << std::endl;
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
}

} /* namespace Scripting */
