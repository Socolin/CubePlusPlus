/*
 * AIFollowOwner.cpp
 *
 *  Created on: Aug 6, 2013
 *      Author: yann291
 */

#include "AIFollowOwner.h"

namespace Scripting
{

AIFollowOwner::AIFollowOwner()
    : ownerScript(nullptr)
    , followerScript(nullptr)
{
}

AIFollowOwner::~AIFollowOwner()
{
}

void AIFollowOwner::followOwnerInit(LivingEntityScript* follower)
{
    followerScript = follower;
}
void AIFollowOwner::followOwnerStartFollowing(World::LivingEntity* owner)
{
    ownerScript = owner;
}

} /* namespace Scripting */
