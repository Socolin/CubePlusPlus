#ifndef BLOCKCRAFTINGTABLESCRIPT_H_
#define BLOCKCRAFTINGTABLESCRIPT_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_craftingtable',0);
   SET @scriptId = LAST_INSERT_ID();
   INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,1,'windowId',1);
 */

#define SCRIPTINGPARAM_BLOCK_CRAFTINGTABLE_WINDOWID 1

class BlockCraftingTableScript : public BlockScript
{
public:
    BlockCraftingTableScript();
    virtual ~BlockCraftingTableScript();
    virtual BlockScript* Copy() override;

    virtual void InitParam(int paramId, int param);

    virtual bool OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
private:
    i_windowDataId windowDataId;
};

} /* namespace Scripting */
#endif /* BLOCKCRAFTINGTABLESCRIPT_H_ */
