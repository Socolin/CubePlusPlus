#ifndef MOBZOMBIESCRIPT_H_
#define MOBZOMBIESCRIPT_H_

#include "MobScript.h"

namespace Scripting
{

class MobZombieScript : public MobScript
{
	typedef MobScript parent_type;
public:
	MobZombieScript();
	virtual ~MobZombieScript();

	virtual LivingEntityScript* Copy() override;

	virtual void Init() override;
	virtual void OnUpdateTick() override;
	virtual void OnDeath() override;
	virtual void OnInteract(World::EntityPlayer* player) override;
private:
	void setChild(bool value);
	void setVillager(bool value);

	bool isChild;
	bool isVillager;
	bool isConverting;
};

} /* namespace Scripting */
#endif /* MOBZOMBIESCRIPT_H_ */
