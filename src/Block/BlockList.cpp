#include "BlockList.h"
#include "Database/DatabaseManager.h"
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
    sql::ResultSet* result = db->querry("SELECT * FROM `block` ORDER BY `blockId`");

    if (result == nullptr)
    {
        std::cerr << "ERROR: no block found in database";
        return;
    }

    std::cout
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


BlockList::~BlockList()
{

}

} /* namespace Inventory */
