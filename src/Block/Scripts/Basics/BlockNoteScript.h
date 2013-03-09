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
    virtual ~BlockNoteScript();
    virtual BlockScript* Copy() override;
    virtual void Init(Block::Block* baseBlock) override;
    virtual bool OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ) override;
    virtual Block::TileEntity* CreateNewTileEntity() override;
    virtual bool UseTileEntity() override;
private:
    unsigned char materialId[5];
    const std::wstring soundName[5];
};

} /* namespace Scripting */
#endif /* BLOCKNOTESCRIPT_H_ */
