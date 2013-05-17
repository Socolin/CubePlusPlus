#ifndef FURNACERECIPES_H_
#define FURNACERECIPES_H_

#include <map>
#include <string>

#include "Util/types.h"
#include "Util/Singleton.h"

namespace Inventory
{
class ItemStack;
}

namespace Database
{

class FurnaceRecipes: public Util::Singleton<FurnaceRecipes>
{
    FurnaceRecipes();
    friend class Util::Singleton<FurnaceRecipes>;
public:
    virtual ~FurnaceRecipes();

    void InitInstance() override;

    struct Recipe
    {
        Recipe()
        {
            itemId = 0;
            strictData = false;
            itemData = 0;
            resultId = 0;
            resultData = 0;
            resultQtt = 0;
            experience = 0;
        }

        Recipe(i_item itemId, int itemData, i_item resultId, i_damage resultData, int resultQtt, float experience)
            : itemId(itemId)
            , strictData(itemData != -1)
            , itemData(itemData)
            , resultId(resultId)
            , resultData(resultData)
            , resultQtt(resultQtt)
            , experience(experience)
        {
        }
        i_item itemId;
        bool strictData;
        i_damage itemData;
        i_item resultId;
        i_damage resultData;
        int resultQtt;
        float experience;
    };

    bool IsValidInput(const Inventory::ItemStack* itemStack) const;
    const Recipe& GetRecipe(i_item itemId, i_damage itemData);
private:
    void load();
    std::map<int, Recipe> recipeList;
};

} /* namespace Database */
#endif /* FURNACERECIPES_H_ */
