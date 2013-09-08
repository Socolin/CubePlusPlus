#ifndef AIMAKEBABY_H_
#define AIMAKEBABY_H_

#include "Util/AABB.h"
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
protected:
    AIMakeBaby();
    virtual ~AIMakeBaby();

    void makeBabyInit(LivingEntityScript* script, i_item foodId);
    bool makeBabyCanBeInLove();
    bool makeBabyIsInLove();
    bool makeBabyHasMate();
    bool makeBabyTryFallInLove(const Inventory::ItemStack* item);
    void makeBabyUpdate(World::ScriptedLivingEntity* baseEntity);
    void makeBabyResetInLove();
    virtual void makeBabyInitBabyScript(LivingEntityScript* babyScript);
private:
    void makeBabySetMate(int entityId);
    void makeBabySpawnBaby(World::ScriptedLivingEntity* scriptedEntity);
    void makeBabyFindMate(World::ScriptedLivingEntity* baseEntity);
    void makeBabyUpdateMate(World::ScriptedLivingEntity* baseEntity);
private:
    int makeBabyTimer;
    int makeBabyInLoveTimer;
    int makeBabyMate;
    i_item makeBabyFoodId;
    int makeBabySpawnBabyTimer;
    Util::AABB makeBabySearchMateBoundingBox;
    LivingEntityScript* baseScript;
};

} /* namespace Scripting */
#endif /* AIMAKEBABY_H_ */
