#include "ItemList.h"

#include "Block/BlockList.h"
#include "Database/DatabaseManager.h"
#include "Util/StringUtil.h"
#include "Item.h"
#include "Scripts/Basics/ItemBlockScript.h"

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
            << "maxStackSize" << "\t"
            << "maxDamage" << "\t"
            << "hasSubType" << "\t"
            << "containerId" << "\t"
            << "scriptId" << "\t"
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

        Item* item = new Item(itemId, maxStackSize, maxDamage, hasSubType, containerId, nullptr);
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
