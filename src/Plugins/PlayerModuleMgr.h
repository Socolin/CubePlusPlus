#ifndef PLAYERMODULEMGR_H_
#define PLAYERMODULEMGR_H_

#include "Util/Singleton.h"

#include <map>
#include <string>

namespace Plugin
{
class PlayerModule;
class PlayerModuleMgr: public Util::Singleton<PlayerModuleMgr>
{
    friend Util::Singleton<PlayerModuleMgr>;
public:
    PlayerModuleMgr();
    virtual ~PlayerModuleMgr();
    void RegistersModules();

    int GetModuleId(const std::string& moduleName);
    const std::map<int, PlayerModule*>& GetModuleIdMap() const;

private:
    void registerModule(PlayerModule* module);
private:
    std::map<std::string, PlayerModule*> moduleMap;
    std::map<int, PlayerModule*> moduleIdMap;
    std::map<std::string, int> moduleNameIdMap;
    int currentModuleId;
};

} /* namespace Plugin */
#endif /* PLAYERMODULEMGR_H_ */
