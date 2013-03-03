#include "StairScript.h"

namespace Scripting
{

StairScript::StairScript()
    : BlockScript("stair_script")
{
}

StairScript::~StairScript()
{
}

BlockScript* StairScript::Copy()
{
    return new StairScript(*this);
}

} /* namespace Scripting */
