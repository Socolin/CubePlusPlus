#ifndef SCRIPTMANAGER_H_
#define SCRIPTMANAGER_H_

#include <unordered_map>
#include <string>

namespace Scripting
{

class BlockScript;
class ItemScript;

class ScriptManager
{
public:
    static ScriptManager* GetInstance();
    virtual ~ScriptManager();

    void RegisterAllScripts();

    void RegisterScript(std::string scriptName, BlockScript* script);
    BlockScript* GetBlockScript(std::string scriptName);

    void RegisterScript(std::string scriptName, ItemScript* script);
    ItemScript* GetItemScript(std::string scriptName);
private:
    ScriptManager();
    static ScriptManager* instance;

    std::unordered_map<std::string, BlockScript*> blockScript;
    std::unordered_map<std::string, ItemScript*> itemScript;
};

} /* namespace Scripting */
#endif /* SCRIPTMANAGER_H_ */
