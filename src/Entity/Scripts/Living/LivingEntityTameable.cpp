#include "LivingEntityTameable.h"
#include "Entity/Scripts/LivingEntityScript.h"
#include "Entity/Scripts/ScriptedLivingEntity.h"

namespace Scripting{
LivingEntityTameable::LivingEntityTameable()
	: baseScript(nullptr)
	, scriptedEntity(nullptr)
	, isTamed(false)
	, isSitting(false)
{

}

LivingEntityTameable::~LivingEntityTameable()
{
}

void LivingEntityTameable::EntityTameableInit(LivingEntityScript* script, World::ScriptedLivingEntity* entity)
{
	baseScript = script;
	scriptedEntity = entity;
}

std::wstring LivingEntityTameable::GetOwnerName()
{
	return ownerName;
}

void LivingEntityTameable::SetOwner(std::wstring ownerName)
{
	scriptedEntity->GetMetadataManager()->SetEntityMetadata(17,ownerName);
}

bool LivingEntityTameable::IsTamed()
{
	return isTamed;
}

void LivingEntityTameable::SetTamed(bool value)
{
	char status = scriptedEntity->GetMetadataManager()->GetCharEntityMetadata(16);
	if(value)
	{
		scriptedEntity->GetMetadataManager()->SetEntityMetadata(16, char((status | 0x04) & 0x07));
	}
	else
	{
		scriptedEntity->GetMetadataManager()->SetEntityMetadata(16, char((status & 0xFB) & 0x07));
	}
	isTamed = value;
}

bool LivingEntityTameable::IsSitting()
{
	return isSitting;
}

void LivingEntityTameable::SetSitting(bool value)
{
	char status = scriptedEntity->GetMetadataManager()->GetCharEntityMetadata(16);
	if(value)
	{
		scriptedEntity->GetMetadataManager()->SetEntityMetadata(16, char((status | 0x01) & 0x07));
	}
	else
	{
		scriptedEntity->GetMetadataManager()->SetEntityMetadata(16, char((status & 0xFE) & 0x07));
	}
	isSitting = value;
}

} /* namespace Scripting */

