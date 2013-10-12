/*
 * AIProtectOwner.cpp
 *
 *  Created on: Aug 6, 2013
 *      Author: yann291
 */

#include "AIProtectOwner.h"

namespace Scripting
{

AIProtectOwner::AIProtectOwner()
    : ownerAttackerScript(nullptr)
    , ownerDefenderScript(nullptr)
{
}

AIProtectOwner::~AIProtectOwner()
{
}

void AIProtectOwner::protectOwnerInit(LivingEntityScript* defender)
{
    ownerDefenderScript = defender;
}

void AIProtectOwner::protectOwnerStartDefending(World::LivingEntity* attacker){
    ownerAttackerScript = attacker;
}

} /* namespace Scripting */
