#ifndef AIMAKEBABY_H_
#define AIMAKEBABY_H_

#include "Util/types.h"

namespace Inventory
{
class ItemStack;
}
namespace World
{
class ScriptedLivingEntity;
}
namespace Scripting
{
class LivingEntityScript;
class AIMakeBaby
{
public:
    AIMakeBaby();
    virtual ~AIMakeBaby();

protected:
    void makeBabyInit(LivingEntityScript* script);
    bool makeBabyCanBeInLove();
    bool makeBabyIsInLove();
    bool makeBabyHasMate();
    void makeBabySetBaby(World::ScriptedLivingEntity* baseEntity);
    bool makeBabyTryFallInLove(const Inventory::ItemStack* item);
    void makeBabySetMate(int entityId);
    void makeBabyUpdate(World::ScriptedLivingEntity* baseEntity);
    void makeBabySpawnBaby(World::ScriptedLivingEntity* scriptedEntity);
private:
    void makeBabyFindMate(World::ScriptedLivingEntity* baseEntity);
    void makeBabyUpdateMate(World::ScriptedLivingEntity* baseEntity);
private:
    int makeBabyTimer;
    int makeBabyInLoveTimer;
    int makeBabyMate;
    i_item makeBabyFoodId;
    int makeBabySpawnBabyTimer;
    LivingEntityScript* baseScript;
};

} /* namespace Scripting */
#endif /* AIMAKEBABY_H_ */
