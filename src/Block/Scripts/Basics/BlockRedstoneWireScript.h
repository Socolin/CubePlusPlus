#ifndef BLOCKREDSTONEWIRESCRIPT_H_
#define BLOCKREDSTONEWIRESCRIPT_H_

#include "Block/Scripts/BlockScript.h"

#include <vector>

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_redstonewire',3);
SET @scriptId = LAST_INSERT_ID();
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,1,'redstoneWireBlockId',1);
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,2,'redstoneRepeaterActiveBlockId',1);
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,3,'redstoneRepeaterIdleBlockId',1);
 */
#define SCRIPTINGPARAM_BLOCK_DOOR_BLOCKID 1
#define SCRIPTINGPARAM_BLOCK_DOOR_NEEDREDSTONE 2


class BlockRedstoneWireScript: public BlockScript
{
public:
    BlockRedstoneWireScript();
    virtual ~BlockRedstoneWireScript();
    virtual BlockScript* Copy() override;

    virtual bool CanPlace(World::World* world, int x, i_height y, int z, char face) const override;
    virtual void OnBlockAdded(World::World* world, int x, i_height y, int z) const override;
    virtual void GetBoundingBoxes(int x, int y, int z, i_data data, std::vector<Util::AABB>& bbList) const override;
    virtual void OnNeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const override;
    virtual bool CanProvidePower() const override;
    virtual i_powerlevel GetWeakPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const override;
    virtual i_powerlevel GetStrongPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const override;
    virtual void OnDestroy(World::World* world, int x, i_height y, int z, i_data data) const override;

private:
    void updateAndPropagateCurrentStrength(World::World* world, int x, i_height y, int z);
    void calculateCurrentChanges(World::World* world, int x, i_height y, int z);
    void notifyWireNeighborsOfNeighborChange(World::World* world, int x, i_height y, int z) const;

    bool isPowerProviderOrWire(World::World* world, int x, i_height y, int z, int side) const;
    bool isPoweredOrRepeater(World::World* world, int x, i_height y, int z, int side) const;
    i_powerlevel getMaxCurrentStrength(World::World* world, int x, i_height y, int z, int strength) const;
private:
    std::vector<s_block_position> blocksToUpdate;
    bool providePower;
    i_block redstoneWireBlockId;
    i_block redstoneRepeaterActiveBlockId;
    i_block redstoneRepeaterIdleBlockId;
};

} /* namespace Scripting */
#endif /* BLOCKREDSTONEWIRESCRIPT_H_ */
