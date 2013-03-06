#ifndef BLOCKDOORSCRIPT_H_
#define BLOCKDOORSCRIPT_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

/*
INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_door',1);
SET @scriptId = LAST_INSERT_ID();
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,1,'door_blockId',1);
 */
#define SCRIPTINGPARAM_BLOCK_DOOR_BLOCKID 1

#define SCRIPT_BLOCK_DOOR_ISTOP(metadata) (metadata & 0x8) == 8
class BlockDoorScript: public BlockScript
{
public:
    BlockDoorScript();
    virtual ~BlockDoorScript();
    virtual BlockScript* Copy();
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, unsigned char y, int z, int face, short& blockId, short& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ);
    virtual bool OnUseBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ);
    virtual void InitParam(int paramId, int param);
private:
    int door_blockid;
};

} /* namespace Util */
#endif /* BLOCKDOORSCRIPT_H_ */
