#include "___CLASS_NAME___.h"

namespace Scripting
{

___CLASS_NAME___::___CLASS_NAME___()
    : BlockScript("block____SCRIPT_NAME_LOWER___")
{
}

___CLASS_NAME___::~___CLASS_NAME___()
{
}

BlockScript* ___CLASS_NAME___::Copy()
{
    return new ___CLASS_NAME___(*this);
}

} /* namespace Scripting */
