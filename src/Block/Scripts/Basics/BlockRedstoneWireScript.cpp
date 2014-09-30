#include "BlockRedstoneWireScript.h"

#include "Util/AssertUtil.h"
#include "World/World.h"

namespace Scripting
{

BlockRedstoneWireScript::BlockRedstoneWireScript()
        : BlockScript("block_redstonewire"), providePower(true), redstoneWireBlockId(0), redstoneRepeaterActiveBlockId(0), redstoneRepeaterIdleBlockId(0)
{
}

BlockRedstoneWireScript::~BlockRedstoneWireScript()
{
}

BlockScript* BlockRedstoneWireScript::Copy()
{
    return new BlockRedstoneWireScript(*this);
}

bool BlockRedstoneWireScript::CanPlace(World::World* world, int x, i_height y, int z, char /*face*/) const
{
    if (y == 0)
        return false;

    s_block_data blockBotomInfo = world->GetBlockIdAndData(x, y - 1, z);
    const Block::Block* blockBottom = Block::BlockList::getBlock(blockBotomInfo.blockId);
    if (blockBottom)
    {
        return blockBottom->HasSolidTopSurface(blockBotomInfo.blockData);
    }
    return false;
}

void BlockRedstoneWireScript::OnBlockAdded(World::World* world, int x, i_height y, int z, i_data /*data*/) const
{
    const_cast<BlockRedstoneWireScript*>(this)->updateAndPropagateCurrentStrength(world, x, y, z);
    world->NotifyNeighborsForBlockChange(x, y + 1, z, baseBlock->GetBlockId());
    world->NotifyNeighborsForBlockChange(x, y - 1, z, baseBlock->GetBlockId());
    notifyWireNeighborsOfNeighborChange(world, x - 1, y, z);
    notifyWireNeighborsOfNeighborChange(world, x + 1, y, z);
    notifyWireNeighborsOfNeighborChange(world, x, y, z - 1);
    notifyWireNeighborsOfNeighborChange(world, x, y, z + 1);

    if (world->IsNormalCube(x - 1, y, z))
    {
        notifyWireNeighborsOfNeighborChange(world, x - 1, y + 1, z);
    }
    else
    {
        notifyWireNeighborsOfNeighborChange(world, x - 1, y - 1, z);
    }

    if (world->IsNormalCube(x + 1, y, z))
    {
        notifyWireNeighborsOfNeighborChange(world, x + 1, y + 1, z);
    }
    else
    {
        notifyWireNeighborsOfNeighborChange(world, x + 1, y - 1, z);
    }

    if (world->IsNormalCube(x, y, z - 1))
    {
        notifyWireNeighborsOfNeighborChange(world, x, y + 1, z - 1);
    }
    else
    {
        notifyWireNeighborsOfNeighborChange(world, x, y - 1, z - 1);
    }

    if (world->IsNormalCube(x, y, z + 1))
    {
        notifyWireNeighborsOfNeighborChange(world, x, y + 1, z + 1);
    }
    else
    {
        notifyWireNeighborsOfNeighborChange(world, x, y - 1, z + 1);
    }
}

void BlockRedstoneWireScript::OnDestroy(World::World* world, int x, i_height y, int z, i_data /*data*/) const
{
    i_block blockId = baseBlock->GetBlockId();
    world->NotifyNeighborsForBlockChange(x, y + 1, z, blockId);
    world->NotifyNeighborsForBlockChange(x, y - 1, z, blockId);
    world->NotifyNeighborsForBlockChange(x + 1, y, z, blockId);
    world->NotifyNeighborsForBlockChange(x - 1, y, z, blockId);
    world->NotifyNeighborsForBlockChange(x, y, z + 1, blockId);
    world->NotifyNeighborsForBlockChange(x, y, z - 1, blockId);
    const_cast<BlockRedstoneWireScript*>(this)->updateAndPropagateCurrentStrength(world, x, y, z);
    notifyWireNeighborsOfNeighborChange(world, x - 1, y, z);
    notifyWireNeighborsOfNeighborChange(world, x + 1, y, z);
    notifyWireNeighborsOfNeighborChange(world, x, y, z - 1);
    notifyWireNeighborsOfNeighborChange(world, x, y, z + 1);

    i_block currentBlockId = world->GetBlockId(x - 1, y, z);
    const Block::Block* currentBlock = Block::BlockList::getBlock(currentBlockId);
    if (currentBlock && currentBlock->IsNormalCube())
    {
        notifyWireNeighborsOfNeighborChange(world, x - 1, y + 1, z);
    }
    else
    {
        notifyWireNeighborsOfNeighborChange(world, x - 1, y - 1, z);
    }

    currentBlockId = world->GetBlockId(x + 1, y, z);
    currentBlock = Block::BlockList::getBlock(currentBlockId);
    if (currentBlock && currentBlock->IsNormalCube())
    {
        notifyWireNeighborsOfNeighborChange(world, x + 1, y + 1, z);
    }
    else
    {
        notifyWireNeighborsOfNeighborChange(world, x + 1, y - 1, z);
    }

    currentBlockId = world->GetBlockId(x, y, z - 1);
    currentBlock = Block::BlockList::getBlock(currentBlockId);
    if (currentBlock && currentBlock->IsNormalCube())
    {
        notifyWireNeighborsOfNeighborChange(world, x, y + 1, z - 1);
    }
    else
    {
        notifyWireNeighborsOfNeighborChange(world, x, y - 1, z - 1);
    }

    currentBlockId = world->GetBlockId(x, y, z + 1);
    currentBlock = Block::BlockList::getBlock(currentBlockId);
    if (currentBlock && currentBlock->IsNormalCube())
    {
        notifyWireNeighborsOfNeighborChange(world, x, y + 1, z + 1);
    }
    else
    {
        notifyWireNeighborsOfNeighborChange(world, x, y - 1, z + 1);
    }
}

void BlockRedstoneWireScript::GetBoundingBoxes(int /*x*/, int /*y*/, int /*z*/, i_data /*data*/, std::vector<Util::AABB>& /*bbList*/) const
{
}

void BlockRedstoneWireScript::OnNeighborChange(World::World* world, int x, i_height y, int z, i_block /*neighborBlockId*/) const
{
    bool canStay = CanPlace(world, x, y, z, 0);

    if (canStay)
    {
        const_cast<BlockRedstoneWireScript*>(this)->updateAndPropagateCurrentStrength(world, x, y, z);
    }
    else
    {
        world->ChangeBlock(x, y, z, 0, 0, false);
    }
}

bool BlockRedstoneWireScript::CanProvidePower() const
{
    return providePower;
}

i_powerlevel BlockRedstoneWireScript::GetWeakPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const
{
    if (!providePower)
        return 0;

    if (metadata == 0)
    {
        return 0;
    }
    else if (side == 1)
    {
        return metadata;
    }
    else
    {
        bool isWestBlockPowered = isPoweredOrRepeater(world, x - 1, y, z, 1) || (!world->IsNormalCube(x - 1, y, z) && isPoweredOrRepeater(world, x - 1, y - 1, z, -1));
        bool isEaseBlockPowered = isPoweredOrRepeater(world, x + 1, y, z, 3) || (!world->IsNormalCube(x + 1, y, z) && isPoweredOrRepeater(world, x + 1, y - 1, z, -1));
        bool isNorthBlockPowered = isPoweredOrRepeater(world, x, y, z - 1, 2) || (!world->IsNormalCube(x, y, z - 1) && isPoweredOrRepeater(world, x, y - 1, z - 1, -1));
        bool isSouthBlockPowered = isPoweredOrRepeater(world, x, y, z + 1, 0) || (!world->IsNormalCube(x, y, z + 1) && isPoweredOrRepeater(world, x, y - 1, z + 1, -1));

        if (!world->IsNormalCube(x, y + 1, z))
        {
            if (world->IsNormalCube(x - 1, y, z) && isPoweredOrRepeater(world, x - 1, y + 1, z, -1))
            {
                isWestBlockPowered = true;
            }

            if (world->IsNormalCube(x + 1, y, z) && isPoweredOrRepeater(world, x + 1, y + 1, z, -1))
            {
                isEaseBlockPowered = true;
            }

            if (world->IsNormalCube(x, y, z - 1) && isPoweredOrRepeater(world, x, y + 1, z - 1, -1))
            {
                isNorthBlockPowered = true;
            }

            if (world->IsNormalCube(x, y, z + 1) && isPoweredOrRepeater(world, x, y + 1, z + 1, -1))
            {
                isSouthBlockPowered = true;
            }
        }

        if (!isNorthBlockPowered && !isEaseBlockPowered && !isWestBlockPowered && !isSouthBlockPowered && side >= 2 && side <= 5)
            return metadata;
        if (side == 2 && isNorthBlockPowered && !isWestBlockPowered && !isEaseBlockPowered)
            return metadata;
        if (side == 3 && isSouthBlockPowered && !isWestBlockPowered && !isEaseBlockPowered)
            return metadata;
        if (side == 4 && isWestBlockPowered && !isNorthBlockPowered && !isSouthBlockPowered)
            return metadata;
        if (side == 5 && isEaseBlockPowered && !isNorthBlockPowered && !isSouthBlockPowered)
            return metadata;
    }
    return 0;
}

i_powerlevel BlockRedstoneWireScript::GetStrongPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const
{
    return GetWeakPowerLevel(world, x, y, z, side, metadata);
}

void BlockRedstoneWireScript::updateAndPropagateCurrentStrength(World::World* world, int x, i_height y, int z)
{
    calculateCurrentChanges(world, x, y, z);
    std::vector<s_block_position> updateList(blocksToUpdate);
    blocksToUpdate.clear();

    for (s_block_position pos : updateList)
    {
        world->NotifyNeighborsForBlockChange(pos.x, pos.y, pos.z, baseBlock->GetBlockId());
    }
}

void BlockRedstoneWireScript::calculateCurrentChanges(World::World* world, int x, i_height y, int z)
{
    i_data blockMetadata = world->GetBlockData(x, y, z);
    i_powerlevel blockPower = getMaxCurrentStrength(world, x, y, z, 0);
    providePower = false;
    int maxPowerArround = world->getMaxPowerFromBlockArround(x, y, z);
    providePower = true;

    if (maxPowerArround > 0 && maxPowerArround > blockPower - 1)
    {
        blockPower = maxPowerArround;
    }

    int newBlockPower = 0;

    for (int side = 0; side < 4; ++side)
    {
        int sideX = x;
        int sideZ = z;

        if (side == 0)
        {
            sideX = x - 1;
        }

        if (side == 1)
        {
            ++sideX;
        }

        if (side == 2)
        {
            sideZ = z - 1;
        }

        if (side == 3)
        {
            ++sideZ;
        }

        if (sideX != x || sideZ != z)
        {
            newBlockPower = getMaxCurrentStrength(world, sideX, y, sideZ, newBlockPower);
        }

        i_block sideBlockId = world->GetBlockId(sideX, y, sideZ);
        const Block::Block* sideBlock = Block::BlockList::getBlock(sideBlockId);
        i_block currentBlockId = world->GetBlockId(x, y + 1, z);
        const Block::Block* currentBlock = Block::BlockList::getBlock(currentBlockId);
        if ((sideBlock && sideBlock->IsNormalCube()) && (currentBlock == nullptr || !currentBlock->IsNormalCube()))
        {
            if ((sideX != x || sideZ != z))
            {
                newBlockPower = getMaxCurrentStrength(world, sideX, y + 1, sideZ, newBlockPower);
            }
        }
        else if (!(sideBlock && sideBlock->IsNormalCube()) && (sideX != x || sideZ != z))
        {
            newBlockPower = getMaxCurrentStrength(world, sideX, y - 1, sideZ, newBlockPower);
        }
    }

    if (newBlockPower > blockPower)
    {
        blockPower = newBlockPower - 1;
    }
    else if (blockPower > 0)
    {
        --blockPower;
    }
    else
    {
        blockPower = 0;
    }

    if (maxPowerArround > blockPower - 1)
    {
        blockPower = maxPowerArround;
    }

    if (blockMetadata != blockPower)
    {
        world->ChangeDataNotify(x, y, z, blockPower);
        blocksToUpdate.emplace_back(x, y, z);
        blocksToUpdate.emplace_back(x - 1, y, z);
        blocksToUpdate.emplace_back(x + 1, y, z);
        if (y > 0)
            blocksToUpdate.emplace_back(x, y - 1, z);
        if (y < 255)
            blocksToUpdate.emplace_back(x, y + 1, z);
        blocksToUpdate.emplace_back(x, y, z - 1);
        blocksToUpdate.emplace_back(x, y, z + 1);
    }
}
void BlockRedstoneWireScript::notifyWireNeighborsOfNeighborChange(World::World* world, int x, i_height y, int z) const
{
    i_block blockId = baseBlock->GetBlockId();
    if (world->GetBlockId(x, y, z) == blockId)
    {
        world->NotifyNeighborsForBlockChange(x, y, z, blockId);
        world->NotifyNeighborsForBlockChange(x - 1, y, z, blockId);
        world->NotifyNeighborsForBlockChange(x + 1, y, z, blockId);
        world->NotifyNeighborsForBlockChange(x, y, z - 1, blockId);
        world->NotifyNeighborsForBlockChange(x, y, z + 1, blockId);
        if (y > 0)
            world->NotifyNeighborsForBlockChange(x, y - 1, z, blockId);
        if (y < 255)
            world->NotifyNeighborsForBlockChange(x, y + 1, z, blockId);
    }
}

bool BlockRedstoneWireScript::isPowerProviderOrWire(World::World* world, int x, i_height y, int z, int side) const
{
    i_block blockId = world->GetBlockId(x, y, z);

    if (blockId == redstoneWireBlockId)
    {
        return true;
    }
    else if (blockId == 0)
    {
        return false;
    }
    else if (blockId != redstoneRepeaterActiveBlockId && blockId != redstoneRepeaterIdleBlockId)
    {
        const Block::Block* block = Block::BlockList::getBlock(blockId);
        return block != nullptr && block->CanProvidePower() && side != -1;
    }
    else
    {
        const int footInvisibleFaceRemap[] = {2, 3, 0, 1};
        i_data blockMetadata = world->GetBlockData(x, y, z);
        return side == (blockMetadata & 3) || side == footInvisibleFaceRemap[blockMetadata & 3];
    }
}

bool BlockRedstoneWireScript::isPoweredOrRepeater(World::World* world, int x, i_height y, int z, int side) const
{
    if (isPowerProviderOrWire(world, x, y, z, side))
    {
        return true;
    }
    else
    {
        i_block blockId = world->GetBlockId(x, y, z);

        if (blockId == redstoneRepeaterActiveBlockId)
        {
            i_data blockMetadata = world->GetBlockData(x, y, z);
            return side == (blockMetadata & 3);
        }
        else
        {
            return false;
        }
    }
}

void BlockRedstoneWireScript::InitParam(int paramId, int param)
{
    switch (paramId)
    {
    case SCRIPTINGPARAM_BLOCK_REDSTONEWIRE_REDSTONEWIREBLOCKID:
        redstoneWireBlockId = param;
        break;
    case SCRIPTINGPARAM_BLOCK_REDSTONEWIRE_REDSTONEREPEATERACTIVEBLOCKID:
        redstoneRepeaterActiveBlockId = param;
        break;
    case CRIPTINGPARAM_BLOCK_REDSTONEWIRE_REDSTONEREPEATERIDLEBLOCKID:
        redstoneRepeaterIdleBlockId = param;
        break;
    default:
        AssertSwitchBadDefault(paramId)
        break;
    }
}

i_powerlevel BlockRedstoneWireScript::getMaxCurrentStrength(World::World* world, int x, i_height y, int z, int strength) const
{
    if (world->GetBlockId(x, y, z) != baseBlock->GetBlockId())
    {
        return strength;
    }
    else
    {
        int newStrength = world->GetBlockData(x, y, z);
        return newStrength > strength ? newStrength : strength;
    }
}

} /* namespace Scripting */
