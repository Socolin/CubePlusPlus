#include "BlockList.h"
#include "Database/DatabaseManager.h"
#include "Util/StringUtil.h"
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

        Block* block = new Block(blockId, soundList[soundId], lightOpacity,
                lightValue, blockResistance, blockHardness, needsRandomTick,
                slipperiness, isCollidable, isOpaqueCube, materialList[materialId],
                nullptr);
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
