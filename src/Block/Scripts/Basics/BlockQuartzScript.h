#ifndef BLOCKQUARTZSCRIPT_H_
#define BLOCKQUARTZSCRIPT_H_

#include "Block/Scripts/BlockScript.h"
namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_quartz',0);
 */
class BlockQuartzScript : public BlockScript
{
public:
    BlockQuartzScript();
    virtual ~BlockQuartzScript();
    virtual BlockScript* Copy() override;
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
};

} /* namespace Scripting */
#endif /* BLOCKQUARTZSCRIPT_H_ */
