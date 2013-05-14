#ifndef WINDOWFURNACESCRIPT_H_
#define WINDOWFURNACESCRIPT_H_

#include "Window/Script/WindowScript.h"

namespace Scripting
{
/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('window_furnace',0);
 */
class WindowFurnaceScript : public WindowScript
{
    typedef WindowScript parent_type;
public:
    WindowFurnaceScript();
    virtual ~WindowFurnaceScript();
    virtual WindowScript* Copy() const;

    virtual void OnOpenWindow(World::EntityPlayer* player) override;
    virtual void OnOpenWindow(World::EntityPlayer* player, Block::TileEntity* tileEntity) override;

};

} /* namespace Scripting */
#endif /* WINDOWFURNACESCRIPT_H_ */
