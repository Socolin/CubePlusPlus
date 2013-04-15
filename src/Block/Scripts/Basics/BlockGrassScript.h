#ifndef BLOCKGRASSSCRIPT_H_
#define BLOCKGRASSSCRIPT_H_

#include "Block/Scripts/BlockScript.h"
namespace Scripting
{

/*
   INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_grass',2);
   SET @scriptId = LAST_INSERT_ID();
   INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,1,'blockDirtId',1);
   INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,2,'blockGrassId',1);
 */

#define SCRIPTINGPARAM_BLOCK_GRASS_DIRTID 1
#define SCRIPTINGPARAM_BLOCK_GRASS_GRASSID 2

class BlockGrassScript : public BlockScript
{
public:
    BlockGrassScript();
    virtual ~BlockGrassScript();
    virtual BlockScript* Copy() override;
    void OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const;
    virtual void InitParam(int paramId, int param) override;
private:
    i_block dirtBlockId;
    i_block grassBlockId;
};

} /* namespace Scripting */
#endif /* BLOCKGRASSSCRIPT_H_ */
