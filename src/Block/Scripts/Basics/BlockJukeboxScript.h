#ifndef BLOCKJUKEBOXSCRIPT_H_
#define BLOCKJUKEBOXSCRIPT_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_jukebox',0);
 */
class BlockJukeboxScript : public BlockScript
{
public:
    BlockJukeboxScript();
    virtual ~BlockJukeboxScript();
    virtual BlockScript* Copy() override;
    virtual bool OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ) override;
private:
    unsigned char materialId[5];
    const std::wstring soundName[5];
};

} /* namespace Scripting */
#endif /* BLOCKJUKEBOXSCRIPT_H_ */
