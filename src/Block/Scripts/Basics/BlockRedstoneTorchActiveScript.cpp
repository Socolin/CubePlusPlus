#include "BlockRedstoneTorchActiveScript.h"

#include "Block/BlockConstants.h"
#include "Util/AssertUtil.h"
#include "World/World.h"

namespace Scripting
{

BlockRedstoneTorchActiveScript::BlockRedstoneTorchActiveScript()
    : parent_type("block_redstonetorchactive")
    , redstoneTorchIdleBlockId(0)
{
}

BlockRedstoneTorchActiveScript::~BlockRedstoneTorchActiveScript()
{
}

BlockScript* BlockRedstoneTorchActiveScript::Copy()
{
    return new BlockRedstoneTorchActiveScript(*this);
}

void BlockRedstoneTorchActiveScript::InitParam(int paramId, int param)
{
    switch (paramId)
    {
    case SCRIPTINGPARAM_BLOCK_REDSTONETORCHACTIVE_REDSTONETORCHIDLEBLOCKID:
        redstoneTorchIdleBlockId = param;
        break;
    default:
        AssertSwitchBadDefault(paramId)
        break;
    }
}

void BlockRedstoneTorchActiveScript::OnBlockAdded(World::World* world, int x, i_height y, int z, i_data data) const
{
    parent_type::OnBlockAdded(world, x, y, z, data);

    FOR_EACH_SIDE_YXZ(x, y, z, BlockSide)
        world->NotifyNeighborsForBlockChange(BlockSideX, BlockSideY, BlockSideZ, baseBlock->GetBlockId());
    }
}

void BlockRedstoneTorchActiveScript::OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const
{
    if (isIndirectlyPowered(world, x, y, z, data))
    {
        world->ChangeBlock(x, y, z, redstoneTorchIdleBlockId, data, false);
        world->GetRedstoneTorchBurnoutMgr()->ChangeTorchState(x, y, z);
    }
}

void BlockRedstoneTorchActiveScript::OnNeighborChange(World::World* world, int x, i_height y, int z, i_block /*neighborBlockId*/) const
{
    // TODO: check can stay with parent_type
    i_data data = world->GetBlockData(x, y, z);
    if (isIndirectlyPowered(world, x, y, z, data))
    {
        world->MarkBlockForUpdate(x, y, z, baseBlock->GetBlockId(), 2);
    }
}

void BlockRedstoneTorchActiveScript::OnDestroy(World::World* world, int x, i_height y, int z, i_data /*data*/) const
{
    FOR_EACH_SIDE_YXZ(x, y, z, BlockSide)
        world->NotifyNeighborsForBlockChange(BlockSideX, BlockSideY, BlockSideZ, baseBlock->GetBlockId());
    END_FOR_EACH_SIDE
}

i_powerlevel BlockRedstoneTorchActiveScript::GetWeakPowerLevel(World::World* /*world*/, int /*x*/, i_height /*y*/, int /*z*/, int side, i_data metadata) const
{
    if (metadata == 5 && side == 1)
        return 0;
    if (metadata == 3 && side == 3)
        return 0;
    if (metadata == 4 && side == 2)
        return 0;
    if (metadata == 1 && side == 5)
        return 0;
    if (metadata == 2 && side == 4)
        return 0;
    return 15;
}

i_powerlevel BlockRedstoneTorchActiveScript::GetStrongPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const
{
    if (side == 0)
        return GetWeakPowerLevel(world, x, y, z, side, metadata);
    return 0;
}


} /* namespace Scripting */
