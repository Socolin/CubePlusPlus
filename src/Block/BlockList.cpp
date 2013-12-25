#include "BlockList.h"

#include "Logging/Logger.h"
#include "Database/DatabaseManager.h"
#include "Util/StringUtil.h"
#include "Scripts/BlockScript.h"
#include "Scripting/ScriptManager.h"
#include <sstream>

namespace Block
{

BlockList::BlockList()
{
    for (int i = 0; i < BLOCK_COUNT; i++)
        blocks[i] = nullptr;
}

BlockList::~BlockList()
{
    for (int i = 0; i < BLOCK_COUNT; i++)
    {
        if (blocks[i] != nullptr)
        {
            delete blocks[i];
            blocks[i] = nullptr;
        }
    }
}

void BlockList::InitInstance()
{
    Initialize();
}

void BlockList::Initialize()
{
    Database::DatabaseManager* db = Database::DatabaseManager::Instance();
    db->connect();
    LoadSounds();
    LoadMaterials();

    sql::ResultSet* result = db->querry("SELECT * FROM `block` ORDER BY `blockId`");

    if (result == nullptr)
    {
        LOG_ERROR << "ERROR: no block found in database";
        return;
    }

    LOG_INFO << "Loading block's datas" << std::endl;
    LOG_DEBUG << Logging::BOLD_BLUE
              << "blockId" << "\t"
              << "lightOp" << "\t"
              << "lightVa" << "\t"
              << "blockRe" << "\t"
              << "blockHa" << "\t"
              << "needsRa" << "\t"
              << "slipper" << "\t"
              << "isColli" << "\t"
              << "isOpaqu" << "\t"
              << "renderA" << "\t"
              << "minX" << "\t"
              << "minY" << "\t"
              << "minZ" << "\t"
              << "maxX" << "\t"
              << "maxY" << "\t"
              << "maxZ" << "\t"
              << "materia" << "\t"
              << "scriptI" << "\t"
              << "soundId" << "\t"
              << "burning" << "\t"
              << "useNeighborBrightness" << "\t"
              << "name" << " "
              << std::endl;
    while(result->next())
    {
        std::string name = result->getString(TableBlock::name);
        i_block blockId = result->getUInt(TableBlock::blockId);
        int soundId = result->getInt(TableBlock::soundId);
        uint8_t lightOpacity = result->getUInt(TableBlock::lightOpacity);
        uint8_t lightValue = result->getUInt(TableBlock::lightValue);
        float blockResistance = result->getDouble(TableBlock::blockResistance);
        float blockHardness = result->getDouble(TableBlock::blockHardness);
        bool needsRandomTick = result->getBoolean(TableBlock::needsRandomTick);
        float slipperiness = result->getDouble(TableBlock::slipperiness);
        bool isCollidable = result->getBoolean(TableBlock::isCollidable);
        bool isOpaqueCube = result->getBoolean(TableBlock::isOpaqueCube);
        bool renderAsNormal = result->getBoolean(TableBlock::renderAsNormal);
        float minX = result->getDouble(TableBlock::minX);
        float minY = result->getDouble(TableBlock::minY);
        float minZ = result->getDouble(TableBlock::minZ);
        float maxX = result->getDouble(TableBlock::maxX);
        float maxY = result->getDouble(TableBlock::maxY);
        float maxZ = result->getDouble(TableBlock::maxZ);
        int materialId = result->getInt(TableBlock::material);
        int burningTime = result->getInt(TableBlock::burningTime);
        bool useNeighborBrightness = result->getInt(TableBlock::useNeighborBrightness);
        i_block replaceBlockId = result->getInt(TableBlock::replaceBlockId);
        short dropId = result->getInt(TableBlock::dropId);
        char dropDataMask = result->getInt(TableBlock::dropDataMask);
        short dropCountMin = result->getInt(TableBlock::dropCountMin);
        short dropCountMax = result->getInt(TableBlock::dropCountMax);
        int chanceToEncourageFire = result->getInt(TableBlock::chanceToEncourageFire);
        int abilityToCatchFire = result->getInt(TableBlock::abilityToCatchFire);
        int scriptId = result->getInt(TableBlock::scriptId);

        Scripting::BlockScript* script = nullptr;
        if (scriptId > 0)
        {
            Scripting::ScriptManager& scriptManager = Scripting::ScriptManager::Instance();
            std::string scriptName = scriptManager.GetScriptName(scriptId);
            script = scriptManager.GetBlockScript(scriptName);
            LOG_DEBUG << "Use script:" << scriptName << " for block:" << blockId << std::endl;
            if (script != NULL)
            {
                std::ostringstream request_construct;
                request_construct << "SELECT `param`,`valueInt`,`valuefloat`,`valueStr`,`type` FROM `script_data`"
                                  "INNER JOIN script_info ON `script_info`.`scriptId` = `script_data`.`scriptId` AND `script_info`.`paramId` = `script_data`.`param`"
                                  "WHERE `script_info`.`scriptId` = " << scriptId << " AND `stuffId` = " << blockId;

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

        Block* block = new Block(blockId, soundList[soundId], lightOpacity,
                                 lightValue, blockResistance, blockHardness, needsRandomTick,
                                 slipperiness, isCollidable, isOpaqueCube,renderAsNormal,
                                 minX, minY, minZ, maxX, maxY, maxZ,
                                 materialList[materialId],
                                 useNeighborBrightness, burningTime,
                                 script,
                                 replaceBlockId,
                                 dropId, dropDataMask, dropCountMin, dropCountMax,
                                 chanceToEncourageFire, abilityToCatchFire);
        LOG_DEBUG << blockId << "\t"
                  << (int)lightOpacity << "\t"
                  << (int)lightValue << "\t"
                  << blockResistance << "\t"
                  << blockHardness << "\t"
                  << needsRandomTick << "\t"
                  << slipperiness << "\t"
                  << isCollidable << "\t"
                  << isOpaqueCube << "\t"
                  << renderAsNormal << "\t"
                  << minX << "\t"
                  << minY << "\t"
                  << minZ << "\t"
                  << maxX << "\t"
                  << maxY << "\t"
                  << maxZ << "\t"
                  << materialId << "\t"
                  << scriptId << "\t"
                  << soundId << "\t"
                  << burningTime << "\t"
                  << useNeighborBrightness << "\t"
                  << name << " "
                  << std::endl;
        blocks[blockId] = block;
    }

    for (i_block id = 0; id < BLOCK_COUNT; id++)
    {
        if (blocks[id])
        {
            blocks[id]->InitScript();
        }
    }
    delete result;
}

void BlockList::LoadSounds()
{
    Database::DatabaseManager* db = Database::DatabaseManager::Instance();
    db->connect();
    sql::ResultSet* result = db->querry("SELECT * FROM `blocksound` ORDER BY `soundId`");

    if (result == nullptr)
    {
        LOG_ERROR << "ERROR: no block found in database" << std::endl;
        return;
    }

    LOG_DEBUG << "Loading sound's datas" << std::endl;
    LOG_DEBUG << Logging::BOLD_BLUE
              << "soundId" << "\t"
              << "volume" << "\t"
              << "modifie" << "\t"
              << "stepSound" << "\t"
              << "placeSound" << "\t"
              << "breakSound" << "\t"
              << "name" << "\t"
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

        LOG_DEBUG << soundId << "\t"
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

    delete result;
}

void BlockList::LoadMaterials()
{
    Database::DatabaseManager* db = Database::DatabaseManager::Instance();
    db->connect();
    sql::ResultSet* result = db->querry("SELECT * FROM `material` ORDER BY `materialId`");

    if (result == nullptr)
    {
        LOG_ERROR << "ERROR: no material found in database";
        return;
    }


    LOG_DEBUG << "Loading material's datas" << std::endl;
    LOG_DEBUG << Logging::BOLD_BLUE
              << "matId" << "\t"
              << "canBurn" << "\t"
              << "replaca" << "\t"
              << "translu" << "\t"
              << "NoTool" << "\t"
              << "mobilit" << "\t"
              << "solid" << "\t"
              << "liquid" << "\t"
              << "name" << "\t"
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
        bool solid = result->getBoolean(TableBlockMaterial::solid);
        bool liquid = result->getBoolean(TableBlockMaterial::liquid);
        bool blocksMovement = result->getBoolean(TableBlockMaterial::blocksMovement);

        LOG_DEBUG
                << materialId << "\t"
                << canBurn << "\t"
                << replacable << "\t"
                << translucent << "\t"
                << requiresNoTool << "\t"
                << mobilityFlag << "\t"
                << solid << "\t"
                << liquid << "\t"
                << name << "\t"
                << std::endl;

        materialList[materialId] = BlockMaterial(materialId, canBurn, replacable, translucent, requiresNoTool, mobilityFlag, solid, liquid, blocksMovement);//TODO; load in db true and flase
    }

    delete result;
}

} /* namespace Block */
