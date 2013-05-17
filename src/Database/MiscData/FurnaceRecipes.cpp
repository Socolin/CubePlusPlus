#include "FurnaceRecipes.h"

#include "Database/DatabaseManager.h"
#include "Util/StringUtil.h"

namespace Database
{

FurnaceRecipes::FurnaceRecipes()
{
}

FurnaceRecipes::~FurnaceRecipes()
{
}

const FurnaceRecipes::Recipe& FurnaceRecipes::GetRecipe(i_item itemId, i_damage itemData)
{
    int key = itemId;
    key = key << 16;
    key = (key | itemData);
    if (recipeList.find(key) != recipeList.end())
        return recipeList[key];

    key = itemId;
    key = key << 16;
    key = (key | i_damage(-1));
    return recipeList[key];
}

void FurnaceRecipes::InitInstance()
{
    load();
}

void FurnaceRecipes::load()
{
    Database::DatabaseManager* db = Database::DatabaseManager::Instance();
    db->connect();
    sql::ResultSet* result = db->querry("SELECT * FROM `furnace_recipe`");

    if (result == nullptr)
    {
        std::cerr << "ERROR: no furnace recipe found in database" << std::endl;
        return;
    }

    std::cout << "Loading furnace recipes's datas" << std::endl;
    std::cout << UTIL_TEXT_SHELL_BOLD_BLUE
              << "itemId" << "\t"
              << "itemDat" << "\t"
              << "resultI" << "\t"
              << "resultD" << "\t"
              << "resultQ" << "\t"
              << "experi" << "\t"
              << UTIL_TEXT_SHELL_NONE
              << std::endl;

    while(result->next())
    {
        i_item itemId = result->getInt(TableFurnaceRecipe::itemId);
        int itemData = result->getInt(TableFurnaceRecipe::itemData);
        i_item resultId = result->getInt(TableFurnaceRecipe::resultId);
        i_damage resultData = result->getInt(TableFurnaceRecipe::resultData);
        int resultQuantity = result->getInt(TableFurnaceRecipe::resultQuantity);
        float experience = result->getDouble(TableFurnaceRecipe::experience);

        std::cout
                << itemId << "\t"
                << itemData << "\t"
                << resultId << "\t"
                << resultData << "\t"
                << resultQuantity << "\t"
                << experience << "\t"
                << std::endl;

        int key = itemId;
        key = key << 16;
        key = (key | i_damage(itemData));
        recipeList[key] = Recipe(itemId, itemData, resultId, resultData, resultQuantity, experience);
    }
}

} /* namespace Database */
