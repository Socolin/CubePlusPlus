#ifndef LIVINGENTITYTAMEABLE_H_
#define LIVINGENTITYTAMEABLE_H_

#include <string>

namespace World
{
class ScriptedLivingEntity;
}
namespace Scripting
{
class LivingEntityScript;
class LivingEntityTameable
{
public:
	LivingEntityTameable();
	virtual ~LivingEntityTameable();
	void EntityTameableInit(LivingEntityScript* script, World::ScriptedLivingEntity* baseEntity);
	std::wstring GetOwnerName();
	void SetOwner(std::wstring ownerName);
	bool IsTamed();
	void SetTamed(bool value);
	bool IsSitting();
	void SetSitting(bool value);
private:
	std::wstring ownerName;
	LivingEntityScript* baseScript;
	World::ScriptedLivingEntity* scriptedEntity;

};
} /* namespace Scripting */
#endif /* LIVINGENTITYTAMEABLE_H_ */
