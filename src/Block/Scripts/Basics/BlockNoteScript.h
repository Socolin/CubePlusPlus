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
    virtual bool OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
    virtual Block::TileEntity* CreateNewTileEntity(int blockX, i_height blockY, int blockZ) const override;
    virtual bool UseTileEntity() const override;
private:
    unsigned char materialId[5];
    const std::wstring soundName[5];
};

} /* namespace Scripting */
#endif /* BLOCKNOTESCRIPT_H_ */
