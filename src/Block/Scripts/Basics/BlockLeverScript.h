#ifndef BLOCKLEVERSCRIPT_H_
#define BLOCKLEVERSCRIPT_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_lever',0);
 */

#define SCRIPT_BLOCK_LEVER_ACTIVATED(metadata) (clickedBlockData & 0x8) == 8
class BlockLeverScript: public BlockScript
{
public:
    BlockLeverScript();
    virtual ~BlockLeverScript();
    virtual BlockScript* Copy() override;
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ) override;
    virtual bool OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ) override;
};

} /* namespace Scripting */
#endif /* BLOCKLEVERSCRIPT_H_ */
