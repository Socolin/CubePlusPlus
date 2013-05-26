#ifndef ___CLASS_NAME_UPPER____H_
#define ___CLASS_NAME_UPPER____H_

#include "Window/Scripts/WindowScript.h"

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('window____SCRIPT_NAME_LOWER___',0);
 */
class ___CLASS_NAME___ : public WindowScript
{
public:
    ___CLASS_NAME___();
    virtual ~___CLASS_NAME___();
    virtual WindowScript* Copy() const override;
___METHODS___
};

} /* namespace Scripting */
#endif /* ___CLASS_NAME_UPPER____H_ */
