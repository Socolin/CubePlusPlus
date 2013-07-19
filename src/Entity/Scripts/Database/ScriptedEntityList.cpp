#include "ScriptedEntityList.h"

#include <sstream>

#include "Database/DatabaseManager.h"
#include "Util/StringUtil.h"
#include "Logging/Logger.h"
#include "Scripting/ScriptManager.h"
#include "Entity/Scripts/LivingEntityScript.h"
#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Util/AssertUtil.h"


namespace World
{

ScriptedEntityList::ScriptedEntityList()
{
    Initialize();
}

ScriptedEntityList::~ScriptedEntityList()
{
}

void ScriptedEntityList::Initialize()
{
    Database::DatabaseManager* db = Database::DatabaseManager::Instance();
    db->connect();

    sql::ResultSet* result = db->querry("SELECT * FROM `entity_living` ORDER BY `id`");

    if (result == nullptr)
    {
        LOG_ERROR << "ERROR: no living entities found in database";
        return;
    }

    LOG_INFO << "Loading living entities's datas" << std::endl;
    LOG_DEBUG << UTIL_TEXT_SHELL_BOLD_BLUE
              << "Id" << "\t"
              << "type" << "\t"
              << "script" << "\t"
              << UTIL_TEXT_SHELL_NONE
              << std::endl;
    while(result->next())
    {
        size_t id = result->getUInt(TableEntityLiving::id);
        char type = result->getUInt(TableEntityLiving::clientType);
        int scriptId = result->getInt(TableEntityLiving::script);

        Scripting::LivingEntityScript* script = nullptr;
        if (scriptId > 0)
        {
            Scripting::ScriptManager& scriptManager = Scripting::ScriptManager::Instance();
            std::string scriptName = scriptManager.GetScriptName(scriptId);
            script = scriptManager.GetLivingEntityScript(scriptName);
            LOG_DEBUG << "Use script:" << scriptName << " for entity:" << id << std::endl;
            if (script != NULL)
            {
                std::ostringstream request_construct;
                request_construct << "SELECT `param`,`valueInt`,`valuefloat`,`valueStr`,`type` FROM `script_data`"
                                  "INNER JOIN script_info ON `script_info`.`scriptId` = `script_data`.`scriptId` AND `script_info`.`paramId` = `script_data`.`param`"
                                  "WHERE `script_info`.`scriptId` = " << scriptId << " AND `stuffId` = " << id;

                sql::ResultSet* script_result = db->querry(request_construct.str());
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
                        AssertSwitchBadDefault(type);
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
        ScriptedEntityLivingData* entityData = new ScriptedEntityLivingData{type, script};
        LOG_DEBUG << id << "\t"
                  << type << "\t"
                  << scriptId << "\t"
                  << std::endl;
        scriptedEntity[id] = entityData;
    }

    delete result;

}

ScriptedEntityLivingData* ScriptedEntityList::GetEntityLivingData(int id)
{
    auto dataItr = scriptedEntity.find(id);
    if (dataItr != scriptedEntity.end())
    {
        return dataItr->second;
    }
    return nullptr;
}

ScriptedLivingEntity* ScriptedEntityList::CreateNewEntity(int id, double x, double y, double z)
{
    ScriptedEntityLivingData* data = GetEntityLivingData(id);
    if (data == nullptr)
    {
        return nullptr;
    }
    Scripting::LivingEntityScript* script = data->baseScript->Copy();
    ScriptedLivingEntity* newEntity = new ScriptedLivingEntity(ENTITY_TYPE_SCRIPTEDLIVING, 0, x, y, z, script, data->type);
    script->Init(newEntity);
    return newEntity;
}

} /* namespace World */
