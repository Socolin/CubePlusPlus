#include "LivingEntityScript.h"

#include "Scripting/ScriptManager.h"

namespace Scripting
{

LivingEntityScript::LivingEntityScript(const std::string scriptName)
    : baseEntity(nullptr)
{
    ScriptManager::Instance().RegisterScript(scriptName, this);
}

void LivingEntityScript::Init(World::ScriptedLivingEntity* baseEntity)
{
    this->baseEntity = baseEntity;
    Init();
}

LivingEntityScript::~LivingEntityScript()
{
}

void LivingEntityScript::OnUpdateTick()
{
}

void LivingEntityScript::OnReceiveAttack(World::LivingEntity* /*attacker*/, int& /*damage*/)
{
}

void LivingEntityScript::OnInteract(World::EntityPlayer* /*player*/)
{
}

void LivingEntityScript::GetCreatePacket(Network::NetworkPacket& /*packet*/)
{
}

void LivingEntityScript::Init()
{
}

void LivingEntityScript::GetUpdatePacket(Network::NetworkPacket& /*packet*/)
{
}

} /* namespace Scripting */
