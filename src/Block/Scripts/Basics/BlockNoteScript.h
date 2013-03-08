#ifndef BLOCKNOTESCRIPT_H_
#define BLOCKNOTESCRIPT_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_note',0);
 */
class BlockNoteScript : public BlockScript
{
public:
    BlockNoteScript();
    virtual void Init(Block::Block* baseBlock);
    virtual ~BlockNoteScript();
    virtual BlockScript* Copy();
    virtual bool OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ);
    virtual Block::TileEntity* CreateNewTileEntity();
    virtual bool UseTileEntity();
private:
    unsigned char materialId[5];
    const std::wstring soundName[5];
};

} /* namespace Scripting */
#endif /* BLOCKNOTESCRIPT_H_ */
