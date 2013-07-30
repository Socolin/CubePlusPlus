#include "LivingEntityAge.h"
#include "Entity/Scripts/LivingEntityScript.h"
#include "Entity/Scripts/ScriptedLivingEntity.h"

namespace Scripting
{
LivingEntityAge::LivingEntityAge()
	: entityAge(0)
	, baseScript(nullptr)
	, sizeMultiplier(1.0F)
{
}

LivingEntityAge::~LivingEntityAge()
{
}

void LivingEntityAge::entityAgeInit(LivingEntityScript* script)
{
    baseScript = script;
}

void LivingEntityAge::entityAgeGrow(World::ScriptedLivingEntity* baseEntity, int age)
{
	int currentAge = entityAgeGetAge(baseEntity);
	currentAge += age * 20;
	if(currentAge > 0){
		currentAge=0;
	}
	entityAge=currentAge;
	entityAgeSetAge(baseEntity,currentAge);
}

void LivingEntityAge::entityAgeSetAge(World::ScriptedLivingEntity* baseEntity, int age)
{
	baseEntity->GetMetadataManager()->SetEntityMetadata(12, int(age));
}

int LivingEntityAge::entityAgeGetAge(World::ScriptedLivingEntity* baseEntity)
{
	return baseEntity->GetMetadataManager()->GetIntEntityMetadata(12);
}

void LivingEntityAge::entityAgeSetBaby(World::ScriptedLivingEntity* baseEntity)
{
    entityAge = -24000;
    entityAgeSetAge(baseEntity, -24000);
}

bool LivingEntityAge::entityAgeIsBaby()
{
    return entityAge < 0;
}

void LivingEntityAge::entityAgeUpdate(World::ScriptedLivingEntity* baseEntity)
{
	entityAge = entityAgeGetAge(baseEntity);
	if(entityAge < 0){
		entityAgeSetAge(baseEntity, entityAge++);
	}
	else if(entityAge > 0){
		entityAgeSetAge(baseEntity, entityAge--);
	}
}

} /* namespace Scripting */
