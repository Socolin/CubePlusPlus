#include "BlockDoorScript.h"

#include <cmath>

#include "Block/Block.h"
#include "Block/BlockConstants.h"
#include "Block/BlockList.h"
#include "Block/TileEntities/TileEntityNote.h"
#include "Entity/EntityPlayer.h"
#include "Util/AssertUtil.h"
#include "World/Chunk.h"
#include "World/World.h"

namespace Scripting
{

BlockDoorScript::BlockDoorScript() :
        BlockScript("block_door"),
        door_blockid(0),
        need_redstone(false)
{
}

BlockDoorScript::~BlockDoorScript()
{
}

BlockScript* BlockDoorScript::Copy()
{
    return new BlockDoorScript(*this);
}

void BlockDoorScript::OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int /*face*/, i_block& /*blockId*/, i_data& data, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    int playerRotation = (int)(std::floor((player->getYaw() * 4.0 / 360.0) + 0.5)) & 3;

    int leftBlockX = x;
    int leftBlockZ = z;
    int rightBlockX = x;
    int rightBlockZ = z;
    switch (playerRotation)
    {
    case 0:
        data = 1;
        leftBlockX++;
        rightBlockX--;
        break;
    case 1:
        data = 2;
        leftBlockZ++;
        rightBlockZ--;
        break;
    case 2:
        data = 3;
        leftBlockX--;
        rightBlockX++;
        break;
    case 3:
        data = 0;
        leftBlockZ--;
        rightBlockZ++;
        break;
    default:
        AssertSwitchBadDefault(playerRotation)
        break;
    }

    World::World* world = player->getWorld();
    i_block blockTop = world->GetBlockId(x, y + 1, z);
    if (blockTop == 0)
    {
		i_block blockLeft = world->GetBlockId(leftBlockX, y, leftBlockZ);
        i_block blockRight = world->GetBlockId(rightBlockX, y, rightBlockZ);
        i_block blockRightTop = world->GetBlockId(rightBlockX, y + 1, rightBlockZ);
        if (blockLeft == door_blockid || (blockLeft == 0 && (blockRight || blockRightTop)))
            world->ChangeBlockNoEvent(x, y + 1, z, door_blockid, 9);
        else
        {
            world->ChangeBlockNoEvent(x, y + 1, z, door_blockid, 8);
        }
    }
}

bool BlockDoorScript::OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char /*face*/, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
	if (need_redstone)
		return false;

    World::World* world = user->getWorld();
    World::Chunk* chunk = world->GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk == nullptr)
        return false;

    int clickedBlockData = chunk->getDataAt(x & 0xf, y, z & 0xf);
    if (SCRIPT_BLOCK_DOOR_ISTOP(clickedBlockData))
    {
        i_block block = chunk->getBlockAt(x & 0xf, y + 1, z & 0xf);
        if (block == door_blockid)
        {
            i_data bottomBlockData = chunk->getDataAt(x & 0xf, y, z & 0xf);
            world->ChangeDataNoEvent(x, y - 1, z, (bottomBlockData ^ 0x4) & 0x7);
        }
    }
    else
    {
        world->ChangeDataNoEvent(x, y, z, clickedBlockData ^ 0x4);
    }
    return true;
}

bool BlockDoorScript::CanPlace(World::World* world, int x, unsigned char y, int z, char face) const
{
    if (face != FACE_TOP)
        return false;
    if (y == 255)
        return false;
    if (y == 0)
        return false;

    i_block topBlock = world->GetBlockId(x, y + 1, z);
    if (topBlock != 0)
        return false;

    s_block_data bottomBlockData = world->GetBlockIdAndData(x, y - 1, z);
    const Block::Block* bottomBlock = Block::BlockList::getBlock(bottomBlockData.blockId);
    if (bottomBlock->HasSolidTopSurface(bottomBlockData.blockData))
        return true;

    return false;
}

void BlockDoorScript::InitParam(int paramId, int param)
{
    switch(paramId)
    {
    case SCRIPTINGPARAM_BLOCK_DOOR_BLOCKID:
        door_blockid = param;
        break;
    case SCRIPTINGPARAM_BLOCK_DOOR_NEEDREDSTONE:
    	need_redstone = (param != 0);
    	break;
    default:
        AssertSwitchBadDefault(paramId)
        break;
    }
}

void BlockDoorScript::OnDestroy(World::World* world, int x, i_height y, int z, i_data data) const
{
    if (SCRIPT_BLOCK_DOOR_ISTOP(data))
    {
        world->ChangeBlockNoEvent(x, y - 1, z, 0, 0);
    }
    else
    {
        world->ChangeBlockNoEvent(x, y + 1, z, 0, 0);
    }
}

void BlockDoorScript::OnNeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const
{
    i_data data = world->GetBlockData(x, y, z);
    if (SCRIPT_BLOCK_DOOR_ISTOP(data))
    {
        OnNeighborChange(world, x, y - 1, z, neighborBlockId);
    }
    else
    {
        // TODO: check top block

        bool mustDrop = false;
        s_block_data bottomBlockData = world->GetBlockIdAndData(x, y - 1, z);
        const Block::Block* bottomBlock = Block::BlockList::getBlock(bottomBlockData.blockId);
        if (!bottomBlock->HasSolidTopSurface(bottomBlockData.blockData))
        {
            mustDrop = true;
        }

        if (mustDrop)
        {
            // TODO
        }
        else
        {
            const Block::Block* neighborBlock = Block::BlockList::getBlock(neighborBlockId);
            bool isPowered = world->isBlockIndirectlyGettingPowered(x, y, z) || world->isBlockIndirectlyGettingPowered(x, y + 1, z);

            if ((isPowered || (neighborBlock && neighborBlock->CanProvidePower())) && neighborBlockId != baseBlock->GetBlockId())
            {
                onPoweredBlockChange(world, x, y, z, isPowered);
            }
        }
    }
}

void BlockDoorScript::onPoweredBlockChange(World::World* world, int x, int y, int z, bool isPowered) const
{
    int metadata = world->GetBlockData(x, y, z);

    if (SCRIPT_BLOCK_DOOR_ISOPEN(metadata) != isPowered)
    {
        world->ChangeDataNoEvent(x, y, z, metadata ^ 0x4);
    }
}

} /* namespace Scripting */
