#ifndef MOBSCRIPT_H_
#define MOBSCRIPT_H_

#include "Entity/Scripts/LivingEntityScript.h"
#include "AI/AIRandomMove.h"

namespace Scripting
{

class MobScript : public LivingEntityScript, public AIRandomMove
{
    typedef LivingEntityScript parent_type;
public:
    MobScript();
    MobScript(const std::string& scriptName);
    virtual ~MobScript();

    virtual void Init() override;

    virtual LivingEntityScript* Copy() override;

    virtual void OnUpdateTick() override;
    virtual void OnReachDestination() override;
};

} /* namespace Scripting */
#endif /* MOBSCRIPT_H_ */
