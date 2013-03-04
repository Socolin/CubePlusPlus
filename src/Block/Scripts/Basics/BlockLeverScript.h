#ifndef BLOCKLEVERSCRIPT_H_
#define BLOCKLEVERSCRIPT_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_lever',0);
 */
class BlockLeverScript: public Scripting::BlockScript
{
public:
    BlockLeverScript();
    virtual ~BlockLeverScript();
    virtual BlockScript* Copy();
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, unsigned char y, int z, int face, short& blockId, short& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ);
    virtual bool OnUseBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ);
};

} /* namespace Scripting */
#endif /* BLOCKLEVERSCRIPT_H_ */
