#include "ItemList.h"

#include <sstream>

#include "Block/BlockList.h"
#include "Database/DatabaseManager.h"
#include "Util/StringUtil.h"
#include "Item.h"
#include "Logging/Logger.h"
#include "Scripts/Basics/ItemBlockScript.h"
#include "Scripting/ScriptManager.h"

namespace Inventory
{

ItemList::ItemList()
{
    for (int i = 0; i < ITEM_COUNT; i++)
        items[i] = nullptr;
}

ItemList::~ItemList()
{
    for (int i = 0; i < ITEM_COUNT; i++)
    {
        if (items[i] != nullptr)
        {
            delete items[i];
            items[i] = nullptr;
        }
    }
}

void ItemList::InitInstance()
{
    Initialize();
}

void ItemList::Initialize()
{
    GenerateItemBlock();
    Database::DatabaseManager& db = Database::DatabaseManager::Instance();
    db.connect();

    sql::ResultSet* result = db.querry("SELECT * FROM `items` ORDER BY `itemId`");

    if (result == nullptr)
    {
        LOG_ERROR << "ERROR: no items found in database" << std::endl;
        return;
    }

    LOG_INFO << "Loading items's datas" << std::endl;
    LOG_DEBUG << Logging::BOLD_BLUE
              << "itemId" << "\t"
              << "StackSz" << "\t"
              << "MDamage" << "\t"
              << "SubType" << "\t"
              << "ctnerId" << "\t"
              << "burning" << "\t"
              << "script" << "\t"
              << "name" << "\t"
              << std::endl;
    while(result->next())
    {
        std::string name = result->getString(TableItems::name);
        uint16_t itemId = result->getUInt(TableItems::itemId);
        int maxStackSize = result->getInt(TableItems::maxStackSize);
        int maxDamage = result->getInt(TableItems::maxDamage);
        bool hasSubType = result->getBoolean(TableItems::hasSubType);
        int containerId = result->getInt(TableItems::containerId);
        int burningTime = result->getInt(TableItems::burningTime);
        int scriptId = result->getInt(TableItems::script);

        Scripting::ItemScript* script = nullptr;
        if (scriptId > 0)
        {
            Scripting::ScriptManager& scriptManager = Scripting::ScriptManager::Instance();
            std::string scriptName = scriptManager.GetScriptName(scriptId);
            script = scriptManager.GetItemScript(scriptName);
            LOG_DEBUG << "Use script:" << scriptName << " for item:" << itemId << std::endl;
            if (script != NULL)
            {
                std::ostringstream request_construct;
                request_construct << "SELECT `param`,`valueInt`,`valuefloat`,`valueStr`,`type` FROM `script_data`"
                                  "INNER JOIN script_info ON `script_info`.`scriptId` = `script_data`.`scriptId` AND `script_info`.`paramId` = `script_data`.`param`"
                                  "WHERE `script_info`.`scriptId` = " << scriptId << " AND `stuffId` = " << itemId;

                sql::ResultSet* script_result = db.querry(request_construct.str());
                //Load script data
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
                        LOG_DEBUG << "\tparam:" << param << " value:" << valueInt << std::endl;
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
                delete script_result;

            }
            else
            {
                LOG_ERROR << "ERROR: Script:" << scriptName << " not found" << std::endl;
            }
        }
        Item* item = new Item(itemId, maxStackSize, maxDamage, hasSubType, containerId, burningTime, script);
        if (script != nullptr)
            script->Init(item);
        LOG_DEBUG << itemId << "\t"
                  << maxStackSize << "\t"
                  << maxDamage << "\t"
                  << hasSubType << "\t"
                  << containerId << "\t"
                  << burningTime << "\t"
                  << scriptId << "\t"
                  << name << "\t"
                  << std::endl;
        if (items[itemId] != nullptr)
            delete items[itemId];
        items[itemId] = item;
    }

    delete result;

}


void ItemList::GenerateItemBlock()
{
    Block::BlockList& list = Block::BlockList::Instance();
    for (int blockId = 0; blockId < BLOCK_COUNT; ++blockId)
    {
        const Block::Block* block = list.getBlock(blockId);
        if (!block)
            continue;
        Scripting::ItemScript* blockScript = Scripting::ScriptManager::Instance().GetItemScript("item_block");
        blockScript->InitParam(SCRIPTINGPARAM_ITEM_BLOCK_BLOCKID, blockId);
        items[blockId] = new Item(blockId, 64, 0, false, 0, block->GetBurningTime(), blockScript);
    }
}


} /* namespace Inventory */
