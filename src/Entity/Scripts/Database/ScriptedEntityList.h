#ifndef SCRIPTEDENTITYLIST_H_
#define SCRIPTEDENTITYLIST_H_

#include "Util/Singleton.h"
#include <map>

namespace Scripting
{
class LivingEntityScript;
}
namespace World
{
class ScriptedLivingEntity;
struct ScriptedEntityLivingData
{
    char type;
    // width heigh
    // todo: MaxHealth
    Scripting::LivingEntityScript* baseScript;
};
class ScriptedEntityList : public Util::Singleton<ScriptedEntityList>
{
    friend Util::Singleton<ScriptedEntityList>;
public:
    ScriptedEntityList();
    virtual ~ScriptedEntityList();
    void Initialize();
    ScriptedEntityLivingData* GetEntityLivingData(int id);

    ScriptedLivingEntity* CreateNewEntity(int id, double x, double y, double z);
private:

    std::map<size_t, ScriptedEntityLivingData*> scriptedEntity;
};

} /* namespace World */
#endif /* SCRIPTEDENTITYLIST_H_ */
