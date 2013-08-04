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

void LivingEntityAge::EntityAgeInit(LivingEntityScript* script)
{
    baseScript = script;
}

void LivingEntityAge::EntityAgeGrow(World::ScriptedLivingEntity* baseEntity, int age)
{
	int currentAge = EntityAgeGetAge(baseEntity);
	currentAge += age * 20;
	if(currentAge > 0){
		currentAge=0;
	}
	entityAge=currentAge;
	EntityAgeSetAge(baseEntity,currentAge);
}

void LivingEntityAge::EntityAgeSetAge(World::ScriptedLivingEntity* baseEntity, int age)
{
	baseEntity->GetMetadataManager()->SetEntityMetadata(12, int(age));
}

int LivingEntityAge::EntityAgeGetAge(World::ScriptedLivingEntity* baseEntity)
{
	return baseEntity->GetMetadataManager()->GetIntEntityMetadata(12);
}

void LivingEntityAge::EntityAgeSetBaby(World::ScriptedLivingEntity* baseEntity)
{
    entityAge = -24000;
    EntityAgeSetAge(baseEntity, -24000);
}

bool LivingEntityAge::EntityAgeIsBaby()
{
    return entityAge < 0;
}

void LivingEntityAge::EntityAgeUpdate(World::ScriptedLivingEntity* baseEntity)
{
	entityAge = EntityAgeGetAge(baseEntity);
	if(entityAge < 0){
		EntityAgeSetAge(baseEntity, entityAge++);
	}
	else if(entityAge > 0){
		EntityAgeSetAge(baseEntity, entityAge--);
	}
}

} /* namespace Scripting */
