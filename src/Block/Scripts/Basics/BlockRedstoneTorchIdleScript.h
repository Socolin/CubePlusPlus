#ifndef BLOCKREDSTONETORCHIDLESCRIPT_H_
#define BLOCKREDSTONETORCHIDLESCRIPT_H_

#include "Block/Scripts/Basics/BlockRedstoneTorch.h"

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_redstonetorchidle',0);
SET @scriptId = LAST_INSERT_ID();
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,1,'redstoneTorchActiveBlockId',1);
 */

#define SCRIPTINGPARAM_BLOCK_REDSTONETORCHIDLE_REDSTONETORCHACTIVEBLOCKID 1

class BlockRedstoneTorchIdleScript : public BlockRedstoneTorch
{
    typedef BlockRedstoneTorch parent_type;
public:
    BlockRedstoneTorchIdleScript();
    virtual ~BlockRedstoneTorchIdleScript();
    virtual BlockScript* Copy() override;

    virtual void InitParam(int paramId, int param) override;

    virtual void OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const override;
    virtual void OnNeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const override;
private:
    i_block redstoneTorchActiveBlockId;
};

} /* namespace Scripting */
#endif /* BLOCKREDSTONETORCHIDLESCRIPT_H_ */
