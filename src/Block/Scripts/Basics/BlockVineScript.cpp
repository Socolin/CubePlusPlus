#include "BlockVineScript.h"

#include "Block/BlockConstants.h"
#include "World/World.h"

namespace Scripting
{

BlockVineScript::BlockVineScript()
    : BlockScript("block_vine")
{
}

BlockVineScript::~BlockVineScript()
{
}

BlockScript* BlockVineScript::Copy()
{
    return new BlockVineScript(*this);
}

bool BlockVineScript::CanPlace(World::World* world, int x, i_height y, int z, char face) const
{
    switch (face)
    {
    case FACE_TOP:
        if (y < 255)
        {
            i_block targetBlockId = world->GetBlockId(x, y + 1, z);
            return CanPlaceOnBlock(targetBlockId);
        }
        return false;
    case FACE_NORTH:
    {
        i_block targetBlockId = world->GetBlockId(x, y, z + 1);
        return CanPlaceOnBlock(targetBlockId);
    }
    case FACE_SOUTH:
    {
        i_block targetBlockId = world->GetBlockId(x, y, z - 1);
        return CanPlaceOnBlock(targetBlockId);
    }
    case FACE_WEST:
    {
        i_block targetBlockId = world->GetBlockId(x + 1, y, z);
        return CanPlaceOnBlock(targetBlockId);
    }
    case FACE_EAST:
    {
        i_block targetBlockId = world->GetBlockId(x - 1, y, z);
        return CanPlaceOnBlock(targetBlockId);
    }
    default:
        return false;
    }
    return false;
}

void BlockVineScript::OnBlockPlacedBy(World::EntityPlayer* /*player*/, int /*x*/, i_height /*y*/, int /*z*/, int face, i_block& /*blockId*/, i_data& data, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    switch (face)
    {
    case FACE_NORTH:
        data = 1;
        break;
    case FACE_SOUTH:
        data = 4;
        break;
    case FACE_WEST:
        data = 8;
        break;
    case FACE_EAST:
        data = 2;
        break;
    default:
        break;
    }
}

void BlockVineScript::OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const
{
    if (rand() % 4)
        return;
    int lookRange = 4;
    int maxVineAround = 5;
    bool found5Vine = false;
    int blockX;
    int blockZ;
    int blockY;
    const i_block blockId = baseBlock->GetBlockId();

    for (blockX = x - lookRange; blockX <= x + lookRange; ++blockX)
    {
        for (blockZ = z - lookRange; blockZ <= z + lookRange; ++blockZ)
        {
            for (blockY = y - 1; blockY <= y + 1; ++blockY)
            {
                if (world->GetBlockId(blockX, blockY, blockZ) == blockId)
                {
                    --maxVineAround;

                    if (maxVineAround <= 0)
                    {
                        found5Vine = true;
                        break;
                    }
                }
            }
        }
    }

    const int expandSide = rand() % 6;

    if (expandSide == 1 && y < 255 && world->GetBlockId(x, y + 1, z) == 0)
    {
        if (found5Vine)
        {
            return;
        }

        int randomMask = (rand() % 16) & data;

        if (randomMask > 0)
        {
            FOR_EACH_SIDE_ZXZX(x, z, block)
                if (!CanPlaceOnBlock(world->GetBlockId(blockX, y + 1, blockZ)))
                {
                    randomMask &= ~(1 << side);
                }
            END_FOR_EACH_SIDE

            if (randomMask > 0)
            {
                world->ChangeBlock(x, y + 1, z, blockId, randomMask, false);
            }
        }
    }
    else
    {
        if (expandSide >= 2 && expandSide <= 5 && (data & 1 << expandSide) == 0)
        {
            if (found5Vine)
            {
                return;
            }

            int destBlockX = x + xOffsetsForSidesZXZX[expandSide - 2];
            int destBlockZ = z + zOffsetsForSidesZXZX[expandSide - 2];

            i_block destBlockId = world->GetBlockId(destBlockX, y, destBlockZ);
            const Block::Block* destBlock = Block::BlockList::getBlock(destBlockId);
            if (destBlockId != 0 && destBlock != nullptr)
            {
                if (!destBlock->GetMaterial().isTranslucent() && destBlock->IsRenderAsNormal())
                {
                    world->ChangeDataNotify(x, y, z, data | 1 << expandSide);
                }
            }
            else
            {
                int sideLeft = ((expandSide - 2) + 1) & 3;
                int sideRight = ((expandSide - 2) + 3) & 3;

                if ((data & 1 << sideLeft) != 0 && CanPlaceOnBlock(world->GetBlockId(destBlockX + xOffsetsForSidesZXZX[sideLeft], y, destBlockZ + zOffsetsForSidesZXZX[sideLeft])))
                {
                    world->ChangeBlock(destBlockX, y, destBlockZ, blockId, 1 << sideLeft, false);
                }
                else if ((data & 1 << sideRight) != 0 && CanPlaceOnBlock(world->GetBlockId(destBlockX + xOffsetsForSidesZXZX[sideRight], y, destBlockZ + zOffsetsForSidesZXZX[sideRight])))
                {
                    world->ChangeBlock(destBlockX, y, destBlockZ, blockId, 1 << sideRight);
                }
                else if ((data & 1 << sideLeft) != 0 && world->GetBlockId(destBlockX + xOffsetsForSidesZXZX[sideLeft], y, destBlockZ + zOffsetsForSidesZXZX[sideLeft]) == 0
                        && CanPlaceOnBlock(world->GetBlockId(x + xOffsetsForSidesZXZX[sideLeft], y, z + zOffsetsForSidesZXZX[sideLeft])))
                {
                    world->ChangeBlock(destBlockX + xOffsetsForSidesZXZX[sideLeft], y, destBlockZ + zOffsetsForSidesZXZX[sideLeft], blockId, 1 << ((expandSide + 2) & 3));
                }
                else if ((data & 1 << sideRight) != 0 && world->GetBlockId(destBlockX + xOffsetsForSidesZXZX[sideRight], y, destBlockZ + zOffsetsForSidesZXZX[sideRight]) == 0
                        && CanPlaceOnBlock(world->GetBlockId(x + xOffsetsForSidesZXZX[sideRight], y, z + zOffsetsForSidesZXZX[sideRight])))
                {
                    world->ChangeBlock(destBlockX + xOffsetsForSidesZXZX[sideRight], y, destBlockZ + zOffsetsForSidesZXZX[sideRight], blockId, 1 << ((expandSide + 2) & 3));
                }
                else if (CanPlaceOnBlock(world->GetBlockId(destBlockX, y + 1, destBlockZ)))
                {
                    world->ChangeBlock(destBlockX, y, destBlockZ, blockId, 0);
                }
            }
        }
        else if (y > 1)
        {
            i_block bottomBlockId = world->GetBlockId(x, y - 1, z);

            if (bottomBlockId == 0)
            {
                i_data randomMask = (rand() % 16) & data;

                if (randomMask > 0)
                {
                    world->ChangeBlock(x, y - 1, z, blockId, randomMask, false);
                }
            }
            else if (bottomBlockId == blockId)
            {
                i_data randomMask = (rand() % 16) & data;
                i_data bottomBlockData = world->GetBlockData(x, y - 1, z);

                if (bottomBlockData != (bottomBlockData | randomMask))
                {
                    world->ChangeDataNotify(x, y - 1, z, bottomBlockData | randomMask);
                }
            }
        }
    }
}

void BlockVineScript::GetBoundingBoxes(int /*x*/, int /*y*/, int /*z*/, i_data /*data*/, std::vector<Util::AABB>& /*bbList*/) const
{
    // Nothing
}

void BlockVineScript::OnNeighborChange(World::World* world, int x, i_height y, int z, i_block /*neighborBlockId*/) const
{
    if (!CanStay(world, x, y, z))
    {
        world->RemoveBlock(x, y, z);
    }
}

bool BlockVineScript::CanPlaceOnBlock(i_block blockId) const
{
    const Block::Block* block = Block::BlockList::getBlock(blockId);
    if (block != nullptr)
    {
        return block->IsRenderAsNormal() && block->GetMaterial().BlocksMovement();
    }
    return false;
}

bool BlockVineScript::CanStay(World::World* world, int x, i_height y, int z) const
{
    i_data metadata = world->GetBlockData(x, y, z);
    i_data finalMask = metadata;

    s_block_data topBlockData{0, 0};
    if (y < 255)
        topBlockData = world->GetBlockIdAndData(x, y + 1, z);

    if (metadata)
    {
        FOR_EACH_SIDE_ZXZX(x, z, block)
            int mask = 1 << side;
            i_block targetBlockId = world->GetBlockId(blockX, y, blockZ);
            if ((metadata & mask) && !CanPlaceOnBlock(targetBlockId))
            {
                if (topBlockData.blockId != baseBlock->GetBlockId() || (topBlockData.blockData & mask) == 0)
                {
                    finalMask &= ~mask;
                }
            }
        END_FOR_EACH_SIDE
    }

    if (finalMask == 0 && !CanPlaceOnBlock(topBlockData.blockId))
    {
        return false;
    }

    if (finalMask != metadata)
    {
        world->ChangeDataNotify(x, y, z, finalMask);
    }
    return true;
}

} /* namespace Scripting */
