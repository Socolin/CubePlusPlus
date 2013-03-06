#include "BlockDoorScript.h"

#include <cmath>

#include "Entity/EntityPlayer.h"
#include "Block/BlockConstants.h"
#include "World/Chunk.h"
#include "World/World.h"

namespace Scripting
{

BlockDoorScript::BlockDoorScript() :
        BlockScript("block_door")
{
    door_blockid = 0;
}

BlockDoorScript::~BlockDoorScript()
{
}

BlockScript* BlockDoorScript::Copy()
{
    return new BlockDoorScript(*this);
}

void BlockDoorScript::OnBlockPlacedBy(World::EntityPlayer* player, int x, unsigned char y, int z, int face, short & blockId, short & data, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
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

    int blockLeft = world->GetBlockId(checkDoorX, y, checkDoorZ);

    int block = chunk->getBlockAt(x & 0xf, y + 1, z & 0xf);
    if (block == 0)
    {
        if (blockLeft == door_blockid)
            chunk->ChangeBlock(x & 0xf, y + 1, z & 0xf, door_blockid, 9);
        else
            chunk->ChangeBlock(x & 0xf, y + 1, z & 0xf, door_blockid, 8);

    }
}

bool BlockDoorScript::OnUseBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
{
    World::World* world = user->getWorld();
    World::Chunk* chunk = world->GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk == nullptr)
        return 0;
    int clickedBlockData = chunk->getDataAt(x & 0xf, y, z & 0xf);
    if (SCRIPT_BLOCK_DOOR_ISTOP(clickedBlockData))
    {
        int block = chunk->getBlockAt(x & 0xf, y + 1, z & 0xf);
        if (block == door_blockid)
        {
            int bottomBlockData = chunk->getDataAt(x & 0xf, y, z & 0xf);
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
    }
}

} /* namespace Util */
