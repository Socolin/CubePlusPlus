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
public:
    AIMakeBaby();
    virtual ~AIMakeBaby();

protected:
    void makeBabyInit(LivingEntityScript* script);
    bool makeBabyCanBeInLove();
    bool makeBabyIsInLove();
    bool makeBabyHasMate();
    bool makeBabyTryFallInLove(const Inventory::ItemStack* item);
    void makeBabySetMate(int entityId);
    void makeBabyUpdate(World::ScriptedLivingEntity* baseEntity);
    void makeBabySpawnBaby(World::ScriptedLivingEntity* scriptedEntity);
    void makeBabyResetInLove();
private:
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
