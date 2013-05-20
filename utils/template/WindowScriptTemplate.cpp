#include "___CLASS_NAME___.h"

#include "Entity/EntityPlayer.h"
#include "Window/Window.h"

namespace Scripting
{

___CLASS_NAME___::___CLASS_NAME___()
    : WindowScript("window____SCRIPT_NAME_LOWER___")
{
}

___CLASS_NAME___::~___CLASS_NAME___()
{
}

WindowScript* ___CLASS_NAME___::Copy()
{
    return new ___CLASS_NAME___(*this);
}

} /* namespace Scripting */
