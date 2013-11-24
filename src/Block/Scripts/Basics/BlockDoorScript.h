#ifndef BLOCKDOORSCRIPT_H_
#define BLOCKDOORSCRIPT_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

/*
INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_door',2);
SET @scriptId = LAST_INSERT_ID();
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,1,'door_blockId',1);
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,2,'needRedstone',1);
 */
#define SCRIPTINGPARAM_BLOCK_DOOR_BLOCKID 1
#define SCRIPTINGPARAM_BLOCK_DOOR_NEEDREDSTONE 2

#define SCRIPT_BLOCK_DOOR_ISTOP(metadata) ((metadata & 0x8) == 8)
#define SCRIPT_BLOCK_DOOR_ISOPEN(metadata) ((metadata & 0x4) == 4)

class BlockDoorScript: public BlockScript
{
public:
    BlockDoorScript();
    virtual ~BlockDoorScript();
    virtual BlockScript* Copy() override;
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
    virtual World::ItemUseResult OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
    virtual bool CanPlace(World::World* world, int x, unsigned char y, int z, char face) const override;
    virtual void InitParam(int paramId, int param);
    virtual void OnDestroy(World::World* world, int x, i_height y, int z, i_data data) const override;
    virtual void OnNeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const override;
private:
    void onPoweredBlockChange(World::World* world, int x, int y, int z, bool isPowered) const;
private:
    i_block door_blockid;
    bool need_redstone;
};

} /* namespace Scripting */
#endif /* BLOCKDOORSCRIPT_H_ */
