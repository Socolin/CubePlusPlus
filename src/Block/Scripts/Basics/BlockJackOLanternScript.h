#ifndef BLOCKJACKOLANTERNSCRIPT_H_
#define BLOCKJACKOLANTERNSCRIPT_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{
/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_jackolantern',0);
 */
class BlockJackOLanternScript: public Scripting::BlockScript
{
public:
    BlockJackOLanternScript();
    virtual ~BlockJackOLanternScript();
    virtual BlockScript* Copy() override;
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ) override;
};

} /* namespace Scripting */
#endif /* BLOCKJACKOLANTERNSCRIPT_H_ */
