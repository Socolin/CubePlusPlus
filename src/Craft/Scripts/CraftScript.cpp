#include "CraftScript.h"

#include "Scripting/ScriptManager.h"

namespace Scripting
{

CraftScript::CraftScript(const char* scriptName)
    : baseCraft(nullptr)
{
    ScriptManager::GetInstance()->RegisterScript(scriptName, this);
}

CraftScript::~CraftScript()
{
}

} /* namespace Scripting */
