#include "LivingEntityTameable.h"
#include "Entity/Scripts/LivingEntityScript.h"
#include "Entity/Scripts/ScriptedLivingEntity.h"

namespace Scripting{
LivingEntityTameable::LivingEntityTameable()
	: baseScript(nullptr)
	, scriptedEntity(nullptr)
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
	return scriptedEntity->GetMetadataManager()->GetStringMetadata(17);
}

void LivingEntityTameable::SetOwner(std::wstring ownerName)
{
	scriptedEntity->GetMetadataManager()->SetEntityMetadata(17,ownerName);
}

bool LivingEntityTameable::IsTamed()
{
	char isTame = scriptedEntity->GetMetadataManager()->GetCharEntityMetadata(16);
	return (isTame & 0x4) != 0;
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
}

bool LivingEntityTameable::IsSitting()
{
	char isTame = scriptedEntity->GetMetadataManager()->GetCharEntityMetadata(16);
	return (isTame & 0x1) != 0;
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
}

} /* namespace Scripting */

