#ifndef ___CLASS_NAME_UPPER____H_
#define ___CLASS_NAME_UPPER____H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block____SCRIPT_NAME_LOWER___',0);
 */
class ___CLASS_NAME___ : public BlockScript
{
public:
    ___CLASS_NAME___();
    virtual ~___CLASS_NAME___();
    virtual BlockScript* Copy() override;
___METHODS___
};

} /* namespace Scripting */
#endif /* ___CLASS_NAME_UPPER____H_ */
