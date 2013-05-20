#include "CraftManager.h"

#include <sstream>

#include "Database/DatabaseManager.h"
#include "Util/StringUtil.h"
#include "Scripting/ScriptManager.h"
#include "Craft.h"
#include "Scripts/CraftScript.h"

namespace Craft
{

CraftManager::CraftManager()
{
}

CraftManager::~CraftManager()
{
}

void CraftManager::InitInstance()
{
    load();
}

const std::vector<Craft*>& CraftManager::GetCraftList(int width, int height)
{
    return craftList[CRAFT_KEY(width,height)];
}

void CraftManager::load()
{
    Database::DatabaseManager* db = Database::DatabaseManager::Instance();
    db->connect();

    sql::ResultSet* result = db->querry("SELECT * FROM `craft` ORDER BY `id`");

    if (result == nullptr)
    {
        std::cerr << "ERROR: no craft found in database";
        return;
    }

    std::cout << "Loading craft's datas" << std::endl;
    std::cout << UTIL_TEXT_SHELL_BOLD_BLUE
                << "id" << "\t"
                << "width" << "\t"
                << "height" << "\t"
                << "resultId" << "\t"
                << "resultDa" << "\t"
                << "resultQu" << "\t"
                << "scriptId" << "\t"
                << UTIL_TEXT_SHELL_NONE << std::endl;
    while (result->next())
    {
        int craftId = result->getInt(TableCraft::id);
        int width = result->getInt(TableCraft::width);
        int height = result->getInt(TableCraft::height);
        int resultId = result->getInt(TableCraft::resultId);
        int resultData = result->getInt(TableCraft::resultData);
        int resultQuantity = result->getInt(TableCraft::resultQuantity);
        int scriptId = result->getInt(TableCraft::scriptId);

        Scripting::CraftScript* script = nullptr;
        if (scriptId > 0)
        {
            Scripting::ScriptManager* scriptManager = Scripting::ScriptManager::GetInstance();
            std::string scriptName = scriptManager->GetScriptName(scriptId);
            script = scriptManager->GetCraftScript(scriptName);
            std::cout << "Use script:" << scriptName << " for craft:" << craftId << std::endl;
            if (script != NULL)
            {
                std::ostringstream request_construct;
                request_construct << "SELECT `param`,`valueInt`,`valuefloat`,`valueStr`,`type` FROM `script_data`"
                        "INNER JOIN script_info ON `script_info`.`scriptId` = `script_data`.`scriptId` AND `script_info`.`paramId` = `script_data`.`param`"
                        "WHERE `script_info`.`scriptId` = " << scriptId << " AND `stuffId` = " << craftId;

                sql::ResultSet* script_result = db->querry(request_construct.str());
                //Load script data
                while (script_result->next())
                {
                    int type = script_result->getInt(TableScriptData_U_ScriptInfo::type);
                    int param = script_result->getInt(TableScriptData_U_ScriptInfo::param);
                    switch (type)
                    {
                    case 0: //string
                    {
                        std::string ValueStr = script_result->getString(TableScriptData_U_ScriptInfo::valueStr);
                        script->InitParam(param, ValueStr);
                        break;
                    }
                    case 1: //int
                    {
                        int valueInt = script_result->getInt(TableScriptData_U_ScriptInfo::valueInt);
                        std::cout << "\tparam:" << param << " value:" << valueInt << std::endl;
                        script->InitParam(param, valueInt);
                        break;
                    }
                    case 2: //float
                    {
                        float valueFloat = script_result->getDouble(TableScriptData_U_ScriptInfo::valuefloat);
                        script->InitParam(param, valueFloat);
                        break;
                    }
                    default:
                        // bug TODO: assert
                        break;
                    }
                }

            }
            else
            {
                std::cerr << "ERROR: Script:" << scriptName << " not found" << std::endl;
            }
        }

        Craft* craft = new Craft(width, height, resultId, resultData, resultQuantity);
        std::cout << craftId << "\t" << width << "\t" << height << "\t" << resultId << "\t" << resultData << "\t" << resultQuantity << "\t" << scriptId << "\t" << std::endl;
        craftList[CRAFT_KEY(width,height)].push_back(craft);

        loadCraftSlot(craft, craftId);
    }
}

void CraftManager::loadCraftSlot(Craft* craft, int craftId)
{
    std::ostringstream request_construct;
    request_construct << "SELECT * FROM `craft_slot` WHERE `craftId` = " << craftId << " ORDER BY `id`";

    Database::DatabaseManager* db = Database::DatabaseManager::Instance();
    db->connect();

    sql::ResultSet* result = db->querry(request_construct.str());

    if (result == nullptr)
    {
        std::cerr << "ERROR: no craft slot found in database, craftid:" << craftId << std::endl ;
        return;
    }

    std::cout << "Loading craft's datas" << std::endl;
    std::cout << UTIL_TEXT_SHELL_BOLD_BLUE
               << "id" << "\t"
               << "width" << "\t"
               << "height" << "\t"
               << "resultId" << "\t"
               << "resultDa" << "\t"
               << "resultQu" << "\t"
               << "scriptId" << "\t"
               << UTIL_TEXT_SHELL_NONE << std::endl;
    while (result->next())
    {
       i_item itemId = result->getInt(TableCraftSlot::itemId);
       i_damage itemData = result->getInt(TableCraftSlot::itemData);
       craft->SetNextSlot(itemId, itemData);
    }
}

} /* namespace Craft */
