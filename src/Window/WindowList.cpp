#include "WindowList.h"

#include <sstream>

#include "Database/DatabaseManager.h"
#include "Util/StringUtil.h"
#include "Scripting/ScriptManager.h"
#include "Scripts/WindowScript.h"
#include "WindowStaticData.h"

namespace Window
{

WindowList::WindowList()
{
    for (int i = 0; i < MAX_WINDOW; i++)
        windows[i] = nullptr;
}

WindowList::~WindowList()
{
    for (int i = 0; i < MAX_WINDOW; i++)
    {
        delete windows[i];
        windows[i] = nullptr;
    }
}

void WindowList::InitInstance()
{
    Initialize();
}

void WindowList::Initialize()
{
    Database::DatabaseManager* db = Database::DatabaseManager::Instance();
    db->connect();

    sql::ResultSet* result = db->querry("SELECT * FROM `windows` ORDER BY `id`");

    if (result == nullptr)
    {
        std::cerr << "ERROR: no window found in database";
        return;
    }

    std::cout << "Loading window's datas" << std::endl;
    std::cout << UTIL_TEXT_SHELL_BOLD_BLUE
              << "id" << "\t"
              << "maxSlot" << "\t"
              << "NetSlot" << "\t"
              << "script" << "\t"
              << "clientWindowId" << "\t"
              << "name" << "\t"
              << UTIL_TEXT_SHELL_NONE
              << std::endl;
    while(result->next())
    {
        std::string name = result->getString(TableWindow::name);
        i_windowDataId windowId = result->getUInt(TableWindow::id);
        int maxSlot = result->getInt(TableWindow::maxSlot);
        int networkMaxSlot = result->getInt(TableWindow::networkMaxSlot);
        int clientWindowId = result->getInt(TableWindow::clientWindowId);
        int scriptId = result->getInt(TableWindow::script);

        Scripting::WindowScript* script = nullptr;
        if (scriptId > 0)
        {
            Scripting::ScriptManager* scriptManager = Scripting::ScriptManager::GetInstance();
            std::string scriptName = scriptManager->GetScriptName(scriptId);
            script = scriptManager->GetWindowScript(scriptName);
            std::cout << "Use script:" << scriptName << " for window:" << windowId << std::endl;
            if (script != NULL)
            {
                std::ostringstream request_construct;
                request_construct << "SELECT `param`,`valueInt`,`valuefloat`,`valueStr`,`type` FROM `script_data`"
                                  "INNER JOIN script_info ON `script_info`.`scriptId` = `script_data`.`scriptId` AND `script_info`.`paramId` = `script_data`.`param`"
                                  "WHERE `script_info`.`scriptId` = " << scriptId << " AND `stuffId` = " << windowId;

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
                        // bug TODO: assert
                        break;
                    }
                }
                delete script_result;

            }
            else
            {
                std::cerr << "ERROR: Script:" << scriptName << " not found" << std::endl;
            }
        }

        std::wstring wname;
        Util::StringToWString(wname, name);
        WindowStaticData* data = new WindowStaticData(windowId, maxSlot, networkMaxSlot, wname, clientWindowId, script);
        std::cout << windowId << "\t"
                  << maxSlot << "\t"
                  << networkMaxSlot << "\t"
                  << scriptId << "\t"
                  << clientWindowId << "\t"
                  << name << "\t"
                  << std::endl;
        windows[windowId] = data;
    }

    delete result;
}


} /* namespace Window */
