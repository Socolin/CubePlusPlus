/*
 * AIEatGrass.cpp
 *
 *  Created on: Jul 28, 2013
 *      Author: yann291
 */

#include "AIEatGrass.h"
#include "Entity/Scripts/Living/AnimalSheepScript.h"
#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "World/World.h"

namespace Scripting
{
AIEatGrass::AIEatGrass()
	: baseScript(nullptr)
	, eatGrassTimer(0)
{
}

AIEatGrass::~AIEatGrass()
{
}

void AIEatGrass::EatGrassInit(AnimalSheepScript* script)
{
	baseScript = script;
}

void AIEatGrass::EatGrassStart(World::ScriptedLivingEntity* baseEntity)
{
	eatGrassTimer = 40;
	baseEntity->SetFlag(char(10));
}

void AIEatGrass::EatGrassUpdate(World::ScriptedLivingEntity* baseEntity)
{
	if(eatGrassTimer == 0)
	{
		World::World* world = baseEntity->GetWorld();
		double x = baseEntity->x;
		double y = baseEntity->y;
		double z = baseEntity->z;
		if(world->GetBlockId(x, y, z) == 31)
		{
			world->RemoveBlock(x,y,z);
			baseScript->EatGrassBonus();
		}
		else if(world->GetBlockId(x, y-1, z) == 2)
		{
			world->ChangeBlock(x ,y-1 ,z ,3 ,0 ,false);
			baseScript->EatGrassBonus();
		}
	}
	else
	{
		eatGrassTimer--;
	}
}

} /* namespace Scripting */
