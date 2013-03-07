#include "BlockDoorScript.h"

#include <cmath>

#include "Entity/EntityPlayer.h"
#include "Block/BlockConstants.h"
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

void BlockDoorScript::OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
{
    int playerRotation = (int)(std::floor((player->getYaw() * 4.0 / 360.0) + 0.5)) & 3;

    int checkDoorX = x;
    int checkDoorZ = z;
    switch (playerRotation)
    {
    case 0:
        data = 1;
        checkDoorX++;
        break;
    case 1:
        data = 2;
        checkDoorZ++;
        break;
    case 2:
        data = 3;
        checkDoorX--;
        break;
    case 3:
        data = 0;
        checkDoorZ--;
        break;
    }

    World::World* world = player->getWorld();
    World::Chunk* chunk = world->GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk == nullptr)
        return;

    i_block block = chunk->getBlockAt(x & 0xf, y + 1, z & 0xf);
    if (block == 0)
    {
		i_block blockLeft = world->GetBlockId(checkDoorX, y, checkDoorZ);
        if (blockLeft == door_blockid)
            chunk->ChangeBlock(x & 0xf, y + 1, z & 0xf, door_blockid, 9);
        else
            chunk->ChangeBlock(x & 0xf, y + 1, z & 0xf, door_blockid, 8);
    }
}

bool BlockDoorScript::OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
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
            chunk->ChangeData(x & 0xf, y - 1, z & 0xf, (bottomBlockData ^ 0x4) & 0x7);
        }
    }
    else
    {
        chunk->ChangeData(x & 0xf, y, z & 0xf, clickedBlockData ^ 0x4);
    }
    return true;
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
    }
}

} /* namespace Util */
