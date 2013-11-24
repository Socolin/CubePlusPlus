#ifndef BLOCKLEVERSCRIPT_H_
#define BLOCKLEVERSCRIPT_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_lever',0);
 */

#define SCRIPT_BLOCK_LEVER_ACTIVATED(metadata) (metadata & 0x8) == 8
class BlockLeverScript: public BlockScript
{
public:
    BlockLeverScript();
    virtual ~BlockLeverScript();
    virtual BlockScript* Copy() override;
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
    virtual World::ItemUseResult OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;

    virtual bool CanProvidePower() const override;
    virtual i_powerlevel GetWeakPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const override;
    virtual i_powerlevel GetStrongPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const override;
    virtual void OnDestroy(World::World* world, int x, i_height y, int z, i_data data) const override;
private:
    void notifyNearBlock(World::World* world, int x, i_height y, int z, i_data data) const;
};

} /* namespace Scripting */
#endif /* BLOCKLEVERSCRIPT_H_ */
