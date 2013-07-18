#include "AnimalScript.h"

#include "Network/OpcodeList.h"

namespace Scripting
{

AnimalScript::AnimalScript()
    : LivingEntityScript("entityliving_animal")
{
}

AnimalScript::AnimalScript(const std::string& scriptName)
    : LivingEntityScript(scriptName)
{
}

LivingEntityScript* AnimalScript::Copy()
{
    return new AnimalScript(*this);
}

AnimalScript::~AnimalScript()
{
}

} /* namespace Scripting */
