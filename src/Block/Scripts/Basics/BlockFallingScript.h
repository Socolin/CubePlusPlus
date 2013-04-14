#ifndef BLOCKFALLINGSCRIPT_H_
#define BLOCKFALLINGSCRIPT_H_

#include "Block/Scripts/BlockScript.h"

#include <set>

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_falling',2);
SET @scriptId = LAST_INSERT_ID();
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,1,'traversableBlockList',0);
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,2,'traversableMaterialList',0);
 * traversableMaterialList take a list of block which has a material that can't keep entity on top of it, like water or lava
 */
#define SCRIPTINGPARAM_BLOCK_FALLING_TRAVERSABLEBLOCK 1
#define SCRIPTINGPARAM_BLOCK_FALLING_TRAVERSABLEMATERIAL 2

class BlockFallingScript : public BlockScript
{
public:
    BlockFallingScript();
    virtual ~BlockFallingScript();
    virtual BlockScript* Copy() override;

    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
    virtual void OnNeighborChange(World::World* world, int x, i_height y, int z) const override;

    virtual void OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const override;
    virtual void InitParam(int paramId, const std::string& param) override;
private:
    bool isBlockTraversable(World::World* world, int x, i_height y, int z) const;
    std::set<i_block> traversableBlock;
    std::set<unsigned int> traversableMaterial;
};

} /* namespace Scripting */
#endif /* BLOCKFALLINGSCRIPT_H_ */
