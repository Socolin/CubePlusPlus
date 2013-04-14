#ifndef TORCHSCRIPT_H_
#define TORCHSCRIPT_H_

#include "Block/Scripts/BlockScript.h"
namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_torch',0);
 */
class BlockTorchScript : public BlockScript
{
public:
    BlockTorchScript();
    virtual ~BlockTorchScript();
    virtual BlockScript* Copy() override;
    virtual bool CanPlace(World::World* world, int x, unsigned char y, int z, char face) const override;
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
    virtual void GetBoundingBoxes(int x, int y, int z, i_data data, std::vector<Util::AABB>& bbList) const override;
};

} /* namespace Scripting */
#endif /* TORCHSCRIPT_H_ */
