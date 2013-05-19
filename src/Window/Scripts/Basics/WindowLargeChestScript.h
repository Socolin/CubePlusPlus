#ifndef WINDOWLARGECHESTSCRIPT_H_
#define WINDOWLARGECHESTSCRIPT_H_

#include "WindowChestScript.h"

namespace Scripting
{
/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('window_largechest',0);
 */
class WindowLargeChestScript: public WindowChestScript
{
    typedef WindowChestScript parent_type;
public:
    WindowLargeChestScript();
    virtual ~WindowLargeChestScript();
    virtual WindowScript* Copy() const;

    virtual void OnOpenWindow(World::EntityPlayer* player, Block::TileEntity* tileEntity) override;

};

} /* namespace Scripting */
#endif /* WINDOWLARGECHESTSCRIPT_H_ */
