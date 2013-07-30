#ifndef AIEATGRASS_H_
#define AIEATGRASS_H_

namespace World
{
class ScriptedLivingEntity;
}
namespace Scripting
{
class AnimalSheepScript;
class AIEatGrass
{
public:
	AIEatGrass();
	virtual ~AIEatGrass();

protected:
	void EatGrassInit(AnimalSheepScript* script);
	void EatGrassStart(World::ScriptedLivingEntity* baseEntity);
	void EatGrassUpdate(World::ScriptedLivingEntity* baseEntity);
	bool EatGrassShouldExecute();

private:
	AnimalSheepScript* baseScript;
	int eatGrassTimer;
};
} /* namespace Scripting */
#endif /* AIEATGRASS_H_ */

