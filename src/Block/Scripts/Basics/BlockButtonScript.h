#ifndef BLOCKBUTTONSCRIPT_H_
#define BLOCKBUTTONSCRIPT_H_

#include "Block/Scripts/BlockScript.h"

#include <string>

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_button',0);
 */
#define SCRIPT_BLOCK_BUTTON_ACTIVATED(Metadata) (Metadata & 0x8)
class BlockButtonScript: public Scripting::BlockScript
{
public:
    BlockButtonScript();
    virtual ~BlockButtonScript();
    virtual BlockScript* Copy() override;
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
    virtual bool OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
    virtual bool CanPlace(World::World* world, int x, unsigned char y, int z, char face) const override;
    virtual void OnNeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const override;
    virtual void OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const override;
private:
    std::wstring soundClick;
};

} /* namespace Scripting */
#endif /* BLOCKBUTTONSCRIPT_H_ */
