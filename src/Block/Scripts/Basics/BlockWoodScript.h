#ifndef BLOCKWOODSCRIPT_H_
#define BLOCKWOODSCRIPT_H_

#include "Block/Scripts/BlockScript.h"
namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_wood',0);
 */
class BlockWoodScript : public BlockScript
{
public:
    BlockWoodScript();
    virtual ~BlockWoodScript();
    virtual BlockScript* Copy();
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ);
};

} /* namespace Scripting */
#endif /* BLOCKWOODSCRIPT_H_ */
