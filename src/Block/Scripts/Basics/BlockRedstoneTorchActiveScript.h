#ifndef BLOCKREDSTONETORCHACTIVESCRIPT_H_
#define BLOCKREDSTONETORCHACTIVESCRIPT_H_

#include "Block/Scripts/Basics/BlockRedstoneTorch.h"

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_redstonetorchactive',1);
SET @scriptId = LAST_INSERT_ID();
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,1,'redstoneTorchIdleBlockId',1);
 */

#define SCRIPTINGPARAM_BLOCK_REDSTONETORCHACTIVE_REDSTONETORCHIDLEBLOCKID 1

class BlockRedstoneTorchActiveScript : public BlockRedstoneTorch
{
    typedef BlockRedstoneTorch parent_type;
public:
    BlockRedstoneTorchActiveScript();
    virtual ~BlockRedstoneTorchActiveScript();
    virtual BlockScript* Copy() override;

    virtual void InitParam(int paramId, int param) override;

    virtual void OnBlockAdded(World::World* world, int x, i_height y, int z, i_data data) const override;
    virtual void OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const override;
    virtual void OnNeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const override;
    virtual void OnDestroy(World::World* world, int x, i_height y, int z, i_data data) const override;
    virtual i_powerlevel GetWeakPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const override;
    virtual i_powerlevel GetStrongPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const override;
private:
    i_block redstoneTorchIdleBlockId;
};

} /* namespace Scripting */
#endif /* BLOCKREDSTONETORCHACTIVESCRIPT_H_ */
