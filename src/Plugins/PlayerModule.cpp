#include "PlayerModule.h"

namespace Plugin
{

PlayerModule::PlayerModule(int moduleId)
    : moduleId(moduleId)
{
}

PlayerModule::~PlayerModule()
{
}

int PlayerModule::GetModuleId() const
{
    return moduleId;
}

} /* namespace Plugin */
