#include "BlockRedstoneTorchIdleScript.h"

#include "Util/AssertUtil.h"
#include "World/World.h"

namespace Scripting
{

BlockRedstoneTorchIdleScript::BlockRedstoneTorchIdleScript()
    : parent_type("block_redstonetorchidle")
    , redstoneTorchActiveBlockId(0)
{
}

BlockRedstoneTorchIdleScript::~BlockRedstoneTorchIdleScript()
{
}

BlockScript* BlockRedstoneTorchIdleScript::Copy()
{
    return new BlockRedstoneTorchIdleScript(*this);
}


void BlockRedstoneTorchIdleScript::InitParam(int paramId, int param)
{
    switch (paramId)
    {
    case SCRIPTINGPARAM_BLOCK_REDSTONETORCHIDLE_REDSTONETORCHACTIVEBLOCKID:
        redstoneTorchActiveBlockId = param;
        break;
    default:
        AssertSwitchBadDefault(paramId)
        break;
    }
}

void BlockRedstoneTorchIdleScript::OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const
{
    if (!isIndirectlyPowered(world, x, y, z, data))
    {
        if (!world->GetRedstoneTorchBurnoutMgr()->CheckTorchBurnout(x, y, z))
            world->ChangeBlock(x, y, z, redstoneTorchActiveBlockId, data, false);
    }
}

void BlockRedstoneTorchIdleScript::OnNeighborChange(World::World* world, int x, i_height y, int z, i_block /*neighborBlockId*/) const
{
    // TODO: check can stay with parent_type
    i_data data = world->GetBlockData(x, y, z);
    if (!isIndirectlyPowered(world, x, y, z, data))
    {
        world->MarkBlockForUpdate(x, y, z, baseBlock->GetBlockId(), 2);
    }
}

} /* namespace Scripting */
