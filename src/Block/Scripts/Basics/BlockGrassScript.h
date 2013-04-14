#ifndef BLOCKGRASSSCRIPT_H_
#define BLOCKGRASSSCRIPT_H_

#include "Block/Scripts/BlockScript.h"
namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_grass',0);
 */

#define BLOCKSCRIPT_BLOCKID_DIRT 3 // TODO: set this in param
#define BLOCKSCRIPT_BLOCKID_GRASS 2 // TODO: set this in param

class BlockGrassScript : public BlockScript
{
public:
    BlockGrassScript();
    virtual ~BlockGrassScript();
    virtual BlockScript* Copy() override;
    void OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const;
private:
    i_block dirtBlockId;
    i_block grassBlockId;
};

} /* namespace Scripting */
#endif /* BLOCKGRASSSCRIPT_H_ */
