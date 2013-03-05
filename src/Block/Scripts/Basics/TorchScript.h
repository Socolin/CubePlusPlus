#ifndef TORCHSCRIPT_H_
#define TORCHSCRIPT_H_

#include "Block/Scripts/BlockScript.h"
namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_torch',0);
 */
class TorchScript : public BlockScript
{
public:
    TorchScript();
    virtual ~TorchScript();
    virtual BlockScript* Copy();
    virtual bool CanPlace(World::World* world, int x, unsigned char y, int z, short data);
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, unsigned char y, int z, int face, short& blockId, short& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ);
};

} /* namespace Scripting */
#endif /* TORCHSCRIPT_H_ */
