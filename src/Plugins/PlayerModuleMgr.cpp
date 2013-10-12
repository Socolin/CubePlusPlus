#include "PlayerModuleMgr.h"
#include "PlayerModule.h"

namespace Plugin
{

PlayerModuleMgr::PlayerModuleMgr()
        : currentModuleId(1)
{
    RegistersModules();
}

PlayerModuleMgr::~PlayerModuleMgr()
{
}

void PlayerModuleMgr::RegistersModules()
{
}

int PlayerModuleMgr::GetModuleId(const std::string& moduleName)
{
    auto moduleItr = moduleNameIdMap.find(moduleName);
    if (moduleItr == moduleNameIdMap.end())
        return -1;
    return moduleItr->second;
}

void PlayerModuleMgr::registerModule(PlayerModule* module)
{
    moduleMap[module->GetModuleName()] = module;
    moduleIdMap[module->GetModuleId()] = module;
    moduleNameIdMap[module->GetModuleName()] = module->GetModuleId();
}

const std::map<int, PlayerModule*>& PlayerModuleMgr::GetModuleIdMap() const
{
    return moduleIdMap;
}

} /* namespace Plugin */
