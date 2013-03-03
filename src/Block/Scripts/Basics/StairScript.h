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
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, unsigned char y, int z, int face, short& blockId, short& data);
};

} /* namespace Scripting */
#endif /* STAIRSCRIPT_H_ */
