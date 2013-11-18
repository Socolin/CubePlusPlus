#include "TileEntityChest.h"

#include <NBTField/NBTField.h>

#include "Block/BlockConstants.h"
#include "Inventory/Inventory.h"
#include "World/World.h"
#include "Util/FloatUtil.h"

namespace Block
{

TileEntityChest::TileEntityChest(World::World* world, int blockX, i_height blockY, int blockZ)
    : TileEntity(TILEENTITY_TYPE_CHEST, world, blockX, blockY, blockZ)
{
    inventory = new Inventory::Inventory(27);
}

TileEntityChest::~TileEntityChest()
{
    inventory->CloseInventoryForDelete();
    delete inventory;
}

void TileEntityChest::UpdateTick()
{
}

bool TileEntityChest::NeedUpdate()
{
    return false;
}

void TileEntityChest::GetDataPacket(Network::NetworkPacket& /*packet*/)
{
}

bool TileEntityChest::HasNetworkData()
{
    return false;
}

Inventory::Inventory* TileEntityChest::GetInventory()
{
    return inventory;
}

void TileEntityChest::NotifyPlayerUse(int action)
{
    switch(action)
    {
    case TILEENTITY_PLAYER_OPEN:
        if (inventory->GetPlayerCount() == 0)
        {
            i_block baseBlockId = world->GetBlockId(blockX, blockY, blockZ);
            int xOffset = 0;
            int zOffset = 0;
            if (world->GetBlockId(blockX, blockY, blockZ - 1) == baseBlockId)
            {
                zOffset = -1;
            }
            else if (world->GetBlockId(blockX - 1, blockY, blockZ) == baseBlockId)
            {
                xOffset = -1;
            }
            world->PlayBlockAction(blockX + xOffset, blockY, blockZ + zOffset, 1, 1, world->GetBlockId(blockX, blockY, blockZ), 4);
            world->PlaySound(blockX, blockY, blockZ, L"random.chestopen", 0.5f, 0.9f - Util::randFloat(0.1F), 4);
        }
        break;
    case TILEENTITY_PLAYER_CLOSE:
        if (inventory->GetPlayerCount() == 0)
        {
            i_block baseBlockId = world->GetBlockId(blockX, blockY, blockZ);
            int xOffset = 0;
            int zOffset = 0;
            if (world->GetBlockId(blockX, blockY, blockZ - 1) == baseBlockId)
            {
                zOffset = -1;
            }
            else if (world->GetBlockId(blockX - 1, blockY, blockZ) == baseBlockId)
            {
                xOffset = -1;
            }
            world->PlayBlockAction(blockX + xOffset, blockY, blockZ + zOffset, 1, 0, world->GetBlockId(blockX, blockY, blockZ), 4);
            world->PlaySound(blockX, blockY, blockZ, L"random.chestclosed", 0.5f, 0.9f - Util::randFloat(0.1F), 4);
        }
        break;
    default:
        break;
    }
}

Inventory::Inventory* TileEntityChest::GetSecondInventory()
{
    i_block baseBlockId = world->GetBlockId(blockX, blockY, blockZ);
    FOR_EACH_SIDE_XZ(blockX, blockZ, blockSide)
        i_block blockId = world->GetBlockId(blockSideX, blockY, blockSideZ);
        if (blockId == baseBlockId)
        {
            TileEntity* tileEntity = world->GetTileEntity(blockSideX, blockY, blockSideZ);
            if (tileEntity == nullptr)
                return nullptr;
            return tileEntity->GetInventory();
        }
        // Check double chest
    END_FOR_EACH_SIDE
    return nullptr;
}

TileEntity* TileEntityChest::Create(World::World* world, int blockX, i_height blockY, int blockZ)
{
    return new TileEntityChest(world, blockX, blockY, blockZ);
}

void TileEntityChest::Load(NBT::TagCompound* nbtData)
{
    NBT::TagList* itemList = nbtData->GetTagAs<NBT::TagList>("Items");
    if (itemList)
    {
        inventory->Load(itemList);
    }
}

void TileEntityChest::Save(NBT::TagCompound* /*nbtData*/)
{
    /*FIXME*/
}

const char* TileEntityChest::GetName()
{
    return "Chest";
}

} /* namespace Block */
