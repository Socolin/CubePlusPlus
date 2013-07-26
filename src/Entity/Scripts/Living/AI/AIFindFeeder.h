#ifndef AIFINDFEEDER_H_
#define AIFINDFEEDER_H_

#include <vector>

#include "Util/AABB.h"
#include "Util/types.h"

namespace World
{
class Entity;
class ScriptedLivingEntity;
}
namespace Scripting
{
class LivingEntityScript;
class AIFindFeeder
{
protected:
    AIFindFeeder();
    virtual ~AIFindFeeder();

    void findFeederInit(LivingEntityScript* script, i_item foodId);
    void findFeederUpdate(World::ScriptedLivingEntity* baseEntity);
    void findFeederSearchFeeder(World::ScriptedLivingEntity* baseEntity);
    bool findFeederHasTarget();
    void findFeederResetTarget();
private:
    int findFeederTimer;
    bool findFeederHasFindFeeder;
    i_item findFeederFoodId;
    std::vector<World::Entity*> findFeederSearchList;
    LivingEntityScript* baseScript;
};

} /* namespace Scripting */
#endif /* AIFINDFEEDER_H_ */
