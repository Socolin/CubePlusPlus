#ifndef SCRIPTMANAGER_H_
#define SCRIPTMANAGER_H_

#include <unordered_map>
#include <string>

#include "Util/Singleton.h"

namespace Scripting
{

class BlockScript;
class ItemScript;
class WindowScript;
class CraftScript;

class ScriptManager : public Util::Singleton<ScriptManager>
{
    friend Util::Singleton<ScriptManager>;
    ScriptManager();
public:
    virtual ~ScriptManager();

    void RegisterAllScripts();

    void RegisterScript(std::string scriptName, BlockScript* script);
    BlockScript* GetBlockScript(std::string scriptName);

    void RegisterScript(std::string scriptName, ItemScript* script);
    ItemScript* GetItemScript(std::string scriptName);

    void RegisterScript(std::string scriptName, WindowScript* script);
    WindowScript* GetWindowScript(std::string scriptName);

    void RegisterScript(std::string scriptName, CraftScript* script);
    CraftScript* GetCraftScript(std::string scriptName);

    void LoadScriptsIds();
    std::string GetScriptName(int scriptId);
private:

    std::unordered_map<std::string, BlockScript*> blockScript;
    std::unordered_map<std::string, ItemScript*> itemScript;
    std::unordered_map<std::string, WindowScript*> windowScript;
    std::unordered_map<std::string, CraftScript*> craftScript;
    std::unordered_map<int, std::string> scriptsIds;
};

} /* namespace Scripting */
#endif /* SCRIPTMANAGER_H_ */
