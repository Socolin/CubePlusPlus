#ifndef AIFOLLOWOWNER_H_
#define AIFOLLOWOWNER_H_

namespace World
{
class LivingEntity;
}
namespace Scripting
{
class LivingEntityScript;
class AIFollowOwner
{
public:
    AIFollowOwner();
    virtual ~AIFollowOwner();
protected:
    void followOwnerInit(LivingEntityScript* follower);
    void followOwnerStartFollowing(World::LivingEntity* owner);
private:
    World::LivingEntity* ownerScript;
    LivingEntityScript* followerScript;
};

} /* namespace Scripting */
#endif /* AIFOLLOWOWNER_H_ */
