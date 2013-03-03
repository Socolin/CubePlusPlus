#ifndef STAIRSCRIPT_H_
#define STAIRSCRIPT_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

class StairScript : public BlockScript
{
public:
    StairScript();
    virtual ~StairScript();
    virtual BlockScript* Copy();
};

} /* namespace Scripting */
#endif /* STAIRSCRIPT_H_ */
