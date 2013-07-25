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
    bool makeBabyIsInLove();
    bool makeBabyHasMate();
    bool makeBabyTryFallInLove(const Inventory::ItemStack* item);
    void makeBabySetMate(int entityId);
    void makeBabyUpdate(World::ScriptedLivingEntity* baseEntity);
private:
    void makeBabyFindMate(World::ScriptedLivingEntity* baseEntity);
    void makeBabyUpdateMate(World::ScriptedLivingEntity* baseEntity);
private:
    int makeBabyTimer;
    int makeBabyInLoveTimer;
    int makeBabyMate;
    i_item makeBabyFoodId;
    LivingEntityScript* baseScript;
};

} /* namespace Scripting */
#endif /* AIMAKEBABY_H_ */
