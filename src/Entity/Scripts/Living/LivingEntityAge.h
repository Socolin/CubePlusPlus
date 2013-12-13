#ifndef LIVINGENTITYAGE_H_
#define LIVINGENTITYAGE_H_

namespace World
{
class ScriptedLivingEntity;
}
namespace Scripting
{
class LivingEntityScript;
class LivingEntityAge {
public:
    LivingEntityAge();
    virtual ~LivingEntityAge();
    void EntityAgeInit(LivingEntityScript* script);
    void EntityAgeGrow(World::ScriptedLivingEntity* baseEntity, int age);
    int EntityAgeGetAge(World::ScriptedLivingEntity* baseEntity);
    void EntityAgeSetAge(World::ScriptedLivingEntity* baseEntity, int age);
    void EntityAgeSetBaby(World::ScriptedLivingEntity* baseEntity);
    bool EntityAgeIsBaby();
    void EntityAgeUpdate(World::ScriptedLivingEntity* baseEntity);
protected:
    int entityAge;
    LivingEntityScript* baseScript;
    float sizeMultiplier;
};

} /* namespace Scripting */
#endif /* LIVINGENTITYAGE_H_ */
