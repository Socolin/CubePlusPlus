/*
 * AIBeg.cpp
 *
 *  Created on: Aug 13, 2013
 *      Author: yann291
 */

#include "AIBeg.h"
#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Entity/Scripts/Living/AnimalWolfScript.h"

namespace Scripting
{

AIBeg::AIBeg()
    : wolfScript(nullptr)
    , begTimer(0)
{
}

AIBeg::~AIBeg()
{
}

void AIBeg::begInit(AnimalWolfScript* script)
{
    wolfScript = script;
}

void AIBeg::begStartBegging()
{
    wolfScript->SetBegging(true);
    begTimer = 40 + rand()%40;
}

void AIBeg::begUpdate(/*World::ScriptedLivingEntity* baseEntity*/)
{
    if(wolfScript->IsBegging() && begTimer==0)
    {

    }
    else
    {

    }
}

} /* namespace Scripting */
