#ifndef LIVINGENTITYTAMEABLE_H_
#define LIVINGENTITYTAMEABLE_H_

#include <string>

namespace World
{
class ScriptedLivingEntity;
}
namespace Scripting
{
class LivingEntityScript;
class LivingEntityTameable
{
public:
    LivingEntityTameable();
    virtual ~LivingEntityTameable();
    void EntityTameableInit(LivingEntityScript* script, World::ScriptedLivingEntity* baseEntity);
    std::wstring GetOwnerName();
    void SetOwner(std::wstring ownerName);
    bool IsTamed();
    void SetTamed(bool value);
    bool IsSitting();
    void SetSitting(bool value);
protected:
    std::wstring ownerName;
    bool isTamed;
    bool isSitting;
    World::ScriptedLivingEntity* scriptedEntity;
    LivingEntityScript* baseScript;

};
} /* namespace Scripting */
#endif /* LIVINGENTITYTAMEABLE_H_ */
