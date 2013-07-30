/*
 * LivingEntityAge.h
 *
 *  Created on: Jul 30, 2013
 *      Author: yann291
 */

#ifndef LIVINGENTITYAGE_H_
#define LIVINGENTITYAGE_H_

namespace World
{
class ScriptedLivingEntity;
}
namespace Scripting
{
class LivingEntityScript;
class LivingEntityAge {
public:
	LivingEntityAge();
	virtual ~LivingEntityAge();
	void entityAgeInit(LivingEntityScript* script);
	void entityAgeGrow(World::ScriptedLivingEntity* baseEntity, int age);
	int entityAgeGetAge(World::ScriptedLivingEntity* baseEntity);
	void entityAgeSetAge(World::ScriptedLivingEntity* baseEntity, int age);
	void entityAgeSetBaby(World::ScriptedLivingEntity* baseEntity);
	bool entityAgeIsBaby();
	void entityAgeUpdate(World::ScriptedLivingEntity* baseEntity);
protected:
	int entityAge;
	LivingEntityScript* baseScript;
	float sizeMultiplier;
};

} /* namespace Scripting */
#endif /* LIVINGENTITYAGE_H_ */
