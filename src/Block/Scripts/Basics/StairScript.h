#ifndef STAIRSCRIPT_H_
#define STAIRSCRIPT_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

/*
 *
INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_stair',0);
 */
#define SCRIPTINGPARAM_ITEM_BLOCK_BLOCKID 1
#define SCRIPTINGPARAM_ITEM_BLOCK_USEMETADATA 2

class StairScript : public BlockScript
{
public:
    StairScript();
    virtual ~StairScript();
    virtual BlockScript* Copy();
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ);
};

} /* namespace Scripting */
#endif /* STAIRSCRIPT_H_ */
