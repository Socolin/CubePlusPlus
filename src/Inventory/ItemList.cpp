#include "ItemList.h"

#include <sstream>

#include "Block/BlockList.h"
#include "Database/DatabaseManager.h"
#include "Util/StringUtil.h"
#include "Item.h"
#include "Scripts/Basics/ItemBlockScript.h"
#include "Scripting/ScriptManager.h"

namespace Inventory
{

ItemList* ItemList::instance = nullptr;
ItemList::ItemList()
{

}

void ItemList::Load()
{
    GenerateItemBlock();
    Database::DatabaseManager* db = Database::DatabaseManager::Instance();
    db->connect();

    sql::ResultSet* result = db->querry("SELECT * FROM `items` ORDER BY `itemId`");

    if (result == nullptr)
    {
        std::cerr << "ERROR: no items found in database";
        return;
    }

    std::cout << "Loading items's datas" << std::endl;
    std::cout << UTIL_TEXT_SHELL_BOLD_BLUE
              << "itemId" << "\t"
              << "StackSz" << "\t"
              << "MDamage" << "\t"
              << "SubType" << "\t"
              << "ctnerId" << "\t"
              << "script" << "\t"
              << "name" << "\t"
              << UTIL_TEXT_SHELL_NONE
              << std::endl;
    while(result->next())
    {
        std::string name = result->getString(TableItems::name);
        uint16_t itemId = result->getUInt(TableItems::itemId);
        int maxStackSize = result->getInt(TableItems::maxStackSize);
        int maxDamage = result->getInt(TableItems::maxDamage);
        bool hasSubType = result->getBoolean(TableItems::hasSubType);
        int containerId = result->getInt(TableItems::containerId);
        int scriptId = result->getInt(TableItems::script);

        Scripting::ItemScript* script = nullptr;
        if (scriptId > 0)
        {
            Scripting::ScriptManager* scriptManager = Scripting::ScriptManager::GetInstance();
            std::string scriptName = scriptManager->GetScriptName(scriptId);
            script = scriptManager->GetItemScript(scriptName);
            std::cout << "Use script:" << scriptName << " for item:" << itemId << std::endl;
            if (script != NULL)
            {
                std::ostringstream request_construct;
                request_construct << "SELECT `param`,`valueInt`,`valuefloat`,`valueStr`,`type` FROM `script_data`"
                                  "INNER JOIN script_info ON `script_info`.`scriptId` = `script_data`.`scriptId` AND `script_info`.`paramId` = `script_data`.`param`"
                                  "WHERE `script_info`.`scriptId` = " << scriptId << " AND `stuffId` = " << itemId;

                sql::ResultSet* script_result = db->querry(request_construct.str());
                while (script_result->next())
                {
                    int type = script_result->getInt(TableScriptData_U_ScriptInfo::type);
                    int param = script_result->getInt(TableScriptData_U_ScriptInfo::param);
                    switch (type)
                    {
                    case 0://string
                    {
                        std::string ValueStr = script_result->getString(TableScriptData_U_ScriptInfo::valueStr);
                        script->InitParam(param, ValueStr);
                        break;
                    }
                    case 1://int
                    {
                        int valueInt = script_result->getInt(TableScriptData_U_ScriptInfo::valueInt);
                        std::cout << "\tparam:" << param << " value:" << valueInt << std::endl;
                        script->InitParam(param, valueInt);
                        break;
                    }
                    case 2://float
                    {
                        float valueFloat = script_result->getDouble(TableScriptData_U_ScriptInfo::valuefloat);
                        script->InitParam(param, valueFloat);
                        break;
                    }
                    default:
                        // bug
                        break;
                    }
                }

                //Load script data
            }
        }
        Item* item = new Item(itemId, maxStackSize, maxDamage, hasSubType, containerId, script);
        std::cout << itemId << "\t"
                  << maxStackSize << "\t"
                  << maxDamage << "\t"
                  << hasSubType << "\t"
                  << containerId << "\t"
                  << scriptId << "\t"
                  << name << "\t"
                  << std::endl;
        items[itemId] = item;
    }

}
ItemList::~ItemList()
{
    // TODO Auto-generated destructor stub
}

void ItemList::GenerateItemBlock()
{
    Block::BlockList* list = Block::BlockList::Instance();
    for (int blockId = 0; blockId < BLOCK_COUNT; ++blockId)
    {
        Block::Block* block = list->blocks[blockId];
        if (!block)
            continue;
        Scripting::ItemBlockScript* blockScript = new Scripting::ItemBlockScript();
        blockScript->InitParam(SCRIPTINGPARAM_ITEM_BLOCK_BLOCKID, blockId);
        items[blockId] = new Item(blockId, 64, 0, false, 0, blockScript);
    }
}


} /* namespace Util */
