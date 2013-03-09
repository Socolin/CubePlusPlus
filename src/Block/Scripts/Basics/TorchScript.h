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
    virtual BlockScript* Copy() override;
    virtual bool CanPlace(World::World* world, int x, unsigned char y, int z, char face) override;
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ) override;
};

} /* namespace Scripting */
#endif /* TORCHSCRIPT_H_ */
