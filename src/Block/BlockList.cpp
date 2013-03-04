#include "BlockList.h"
#include "Database/DatabaseManager.h"
#include "Util/StringUtil.h"
#include "Scripts/BlockScript.h"
#include "Scripting/ScriptManager.h"
#include <sstream>

namespace Block
{

BlockList* BlockList::instance = nullptr;
BlockList::BlockList()
{

}

void BlockList::Load()
{
    Database::DatabaseManager* db = Database::DatabaseManager::Instance();
    db->connect();
    LoadSounds();
    LoadMaterials();

    sql::ResultSet* result = db->querry("SELECT * FROM `block` ORDER BY `blockId`");

    if (result == nullptr)
    {
        std::cerr << "ERROR: no block found in database";
        return;
    }

    std::cout << "Loading block's datas" << std::endl;
    std::cout << UTIL_TEXT_SHELL_BOLD_BLUE
              << "blockId" << "\t"
              << "lightOp" << "\t"
              << "lightVa" << "\t"
              << "blockRe" << "\t"
              << "blockHa" << "\t"
              << "needsRa" << "\t"
              << "slipper" << "\t"
              << "isColli" << "\t"
              << "isOpaqu" << "\t"
              << "materia" << "\t"
              << "scriptI" << "\t"
              << "soundId" << "\t"
              << "name" << " "
              << UTIL_TEXT_SHELL_NONE
              << std::endl;
    while(result->next())
    {
        std::string name = result->getString(TableBlock::name);
        uint16_t blockId = result->getUInt(TableBlock::blockId);
        int soundId = result->getInt(TableBlock::soundId);
        uint8_t lightOpacity = result->getUInt(TableBlock::lightOpacity);
        uint8_t lightValue = result->getUInt(TableBlock::lightValue);
        float blockResistance = result->getDouble(TableBlock::blockResistance);
        float blockHardness = result->getDouble(TableBlock::blockHardness);
        bool needsRandomTick = result->getBoolean(TableBlock::needsRandomTick);
        float slipperiness = result->getDouble(TableBlock::slipperiness);
        bool isCollidable = result->getBoolean(TableBlock::isCollidable);
        bool isOpaqueCube = result->getBoolean(TableBlock::isOpaqueCube);
        int materialId = result->getInt(TableBlock::material);
        int scriptId = result->getInt(TableBlock::scriptId);

        Scripting::BlockScript* script = nullptr;
        if (scriptId > 0)
        {
            Scripting::ScriptManager* scriptManager = Scripting::ScriptManager::GetInstance();
            std::string scriptName = scriptManager->GetScriptName(scriptId);
            script = scriptManager->GetBlockScript(scriptName);
            std::cout << "Use script:" << scriptName << " for block:" << blockId << std::endl;
            if (script != NULL)
            {
                std::ostringstream request_construct;
                request_construct << "SELECT `param`,`valueInt`,`valuefloat`,`valueStr`,`type` FROM `script_data`"
                                  "INNER JOIN script_info ON `script_info`.`scriptId` = `script_data`.`scriptId` AND `script_info`.`paramId` = `script_data`.`param`"
                                  "WHERE `script_info`.`scriptId` = " << scriptId << " AND `stuffId` = " << blockId;

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
            else
            {
                std::cerr << "ERROR: Script:" << scriptName << " not found" << std::endl;
            }
        }

        Block* block = new Block(blockId, soundList[soundId], lightOpacity,
                                 lightValue, blockResistance, blockHardness, needsRandomTick,
                                 slipperiness, isCollidable, isOpaqueCube, materialList[materialId],
                                 script);
        std::cout << blockId << "\t"
                  << (int)lightOpacity << "\t"
                  << (int)lightValue << "\t"
                  << blockResistance << "\t"
                  << blockHardness << "\t"
                  << needsRandomTick << "\t"
                  << slipperiness << "\t"
                  << isCollidable << "\t"
                  << isOpaqueCube << "\t"
                  << materialId << "\t"
                  << scriptId << "\t"
                  << soundId << "\t"
                  << name << " "
                  << std::endl;
        blocks[blockId] = block;
    }
}

void BlockList::LoadSounds()
{
    Database::DatabaseManager* db = Database::DatabaseManager::Instance();
    db->connect();
    sql::ResultSet* result = db->querry("SELECT * FROM `blocksound` ORDER BY `soundId`");

    if (result == nullptr)
    {
        std::cerr << "ERROR: no block found in database";
        return;
    }

    std::cout << "Loading sound's datas" << std::endl;
    std::cout << UTIL_TEXT_SHELL_BOLD_BLUE
              << "soundId" << "\t"
              << "volume" << "\t"
              << "modifie" << "\t"
              << "stepSound" << "\t"
              << "placeSound" << "\t"
              << "breakSound" << "\t"
              << "name" << "\t"
              << UTIL_TEXT_SHELL_NONE
              << std::endl;
    while(result->next())
    {
        std::string soundName = result->getString(TableBlockSound::name);
        std::string stepSound = result->getString(TableBlockSound::stepSound);
        std::string placeSound = result->getString(TableBlockSound::placeSound);
        std::string breakSound = result->getString(TableBlockSound::breakSound);
        int soundId = result->getInt(TableBlockSound::soundId);
        float volume = result->getDouble(TableBlockSound::volume);
        float modifier = result->getDouble(TableBlockSound::modifier);

        std::cout << soundId << "\t"
                  << volume << "\t"
                  << modifier << "\t"
                  << stepSound << "\t"
                  << placeSound << "\t"
                  << breakSound << "\t"
                  << soundName << "\t"
                  << std::endl;

        std::wstring wStepSound;
        std::wstring wBreakSound;
        std::wstring wPlaceSound;

        Util::StringToWString(wStepSound, stepSound);
        Util::StringToWString(wBreakSound, breakSound);
        Util::StringToWString(wPlaceSound, placeSound);
        soundList[soundId] = SoundBlock(wStepSound, wBreakSound, wPlaceSound, volume, modifier);
    }
}

void BlockList::LoadMaterials()
{
    Database::DatabaseManager* db = Database::DatabaseManager::Instance();
    db->connect();
    sql::ResultSet* result = db->querry("SELECT * FROM `material` ORDER BY `materialId`");

    if (result == nullptr)
    {
        std::cerr << "ERROR: no material found in database";
        return;
    }


    std::cout << "Loading material's datas" << std::endl;
    std::cout << UTIL_TEXT_SHELL_BOLD_BLUE
              << "matId" << "\t"
              << "canBurn" << "\t"
              << "replaca" << "\t"
              << "translu" << "\t"
              << "NoTool" << "\t"
              << "mobilit" << "\t"
              << "name" << "\t"
              << UTIL_TEXT_SHELL_NONE
              << std::endl;
    while(result->next())
    {
        std::string name = result->getString(TableBlockMaterial::name);
        int materialId = result->getInt(TableBlockMaterial::materialId);
        bool canBurn = result->getBoolean(TableBlockMaterial::canBurn);
        bool replacable = result->getBoolean(TableBlockMaterial::replacable);
        bool translucent = result->getBoolean(TableBlockMaterial::translucent);
        bool requiresNoTool = result->getBoolean(TableBlockMaterial::requiresNoTool);
        int mobilityFlag = result->getInt(TableBlockMaterial::mobilityFlag);

        std::cout
                << materialId << "\t"
                << canBurn << "\t"
                << replacable << "\t"
                << translucent << "\t"
                << requiresNoTool << "\t"
                << mobilityFlag << "\t"
                << name << "\t"
                << std::endl;

        std::wstring wStepSound;
        std::wstring wBreakSound;
        std::wstring wPlaceSound;
        materialList[materialId] = BlockMaterial(canBurn, replacable, translucent, requiresNoTool, mobilityFlag);
    }
}

BlockList::~BlockList()
{

}

} /* namespace Inventory */
