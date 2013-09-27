#include "MobZombieScript.h"

#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "Entity/EntityPlayer.h"
#include "World/World.h"

namespace Scripting
{

MobZombieScript::MobZombieScript()
	: MobScript("entityliving_zombie")
	, isChild(false)
	, isVillager(false)
	, isConverting(false)
{
}

LivingEntityScript* MobZombieScript::Copy()
{
	return new MobZombieScript(*this);
}

MobZombieScript::~MobZombieScript()
{
}

void MobZombieScript::Init()
{
	parent_type::Init();
	baseEntity->SetLivingSound(L"mob.zombie.say");
	baseEntity->SetHurtSound(L"mob.zombie.hurt");
	baseEntity->SetDeathSound(L"mob.zombie.death");
	baseEntity->SetLivingSoundInterval(120);
	baseEntity->SetWidthHeight(0.6, 1.8);
}

void MobZombieScript::OnUpdateTick()
{
	if(!isConverting)
	{
		randomMoveUpdate();
	}
}

void MobZombieScript::OnDeath()
{
	baseEntity->GetWorld()->DropItemstack(*baseEntity, new Inventory::ItemStack(367, 1, 0));
	//TODO : Drop rare items (carrots, gold...)
}

void MobZombieScript::OnInteract(World::EntityPlayer* player)
{
	//TODO : Zombie villager may be healed with gold apple
}

void MobZombieScript::setChild(bool value)
{
	baseEntity->GetMetadataManager()->SetEntityMetadata(12, char(value?1:0));
}

void MobZombieScript::setVillager(bool value)
{
	baseEntity->GetMetadataManager()->SetEntityMetadata(13, char(value?1:0));
}

} /* namespace Scripting */
