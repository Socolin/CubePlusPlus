#include "BlockJukeboxScript.h"

#include "Block/TileEntities/TileEntityRecordPlayer.h"
#include "Block/BlockList.h"
#include "Block/BlockMaterial.h"
#include "Entity/EntityPlayer.h"
#include "Util/AssertUtil.h"
#include "World/World.h"

namespace Scripting
{

BlockJukeboxScript::BlockJukeboxScript()
        : BlockScript("block_jukebox")
{
}

BlockJukeboxScript::~BlockJukeboxScript()
{
}

BlockScript* BlockJukeboxScript::Copy()
{
    return new BlockJukeboxScript(*this);
}
ItemUseResult BlockJukeboxScript::OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char /*face*/, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    World::World* world = user->GetWorld();
    World::Chunk* chunk = world->GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk == nullptr)
    {
        return ItemUseResult{false, false, 0};
    }
    Block::TileEntity* tileEntity = chunk->GetTileEntity(x & 0xf, y, z & 0xf);
    if (tileEntity)
    {
        Block::TileEntityRecordPlayer* recordPlayer = dynamic_cast<Block::TileEntityRecordPlayer*>(tileEntity);

        if (recordPlayer)
        {
            if (recordPlayer->GetRecordItem().LookSlot(0) != nullptr)
            {
                EjectRecord(world, x, y, z);
                return ItemUseResult{true, false, 0};
            }
            else
            {
                Inventory::InventoryPlayer* handInventory = user->GetHandsInventory();
                Inventory::ItemStack* item = handInventory->TakeSomeItemInSlot(handInventory->getHandSlotId(), 1);
                if (item != nullptr)
                {
                    if (item_list.find(item->getItemId()) != item_list.end())
                    {
                        recordPlayer->SetRecordItem(world, x, y, z, item);
                        return ItemUseResult{true, false, 1};
                    }
                }
            }

        }
    }
    return ItemUseResult{false, false, 0};
}

Block::TileEntity* BlockJukeboxScript::CreateNewTileEntity(World::World* world, int blockX, i_height blockY, int blockZ) const
{
    return new Block::TileEntityRecordPlayer(world, blockX, blockY, blockZ);
}

bool BlockJukeboxScript::UseTileEntity() const
{
    return true;
}

void BlockJukeboxScript::OnDestroy(World::World* world, int x, i_height y, int z, i_data /*data*/) const
{
    EjectRecord(world, x, y, z);
}

void BlockJukeboxScript::EjectRecord(World::World* world, int x, i_height y, int z) const
{
    Block::TileEntity* tileEntity = world->GetChunkIfLoaded(x >> 4, z >> 4)->GetTileEntity(x & 0xf, y, z & 0xf);
    if (tileEntity)
    {
        Block::TileEntityRecordPlayer* recordPlayer = dynamic_cast<Block::TileEntityRecordPlayer*>(tileEntity);
        if (recordPlayer)
        {
            world->DropItemstackWithRandomDirection(x + 0.5, y + 1, z + 0.5, recordPlayer->GetRecordItem().TakeSlot(0));
            recordPlayer->SetRecordItem(world, x, y, z, nullptr);
        }
    }
}

void BlockJukeboxScript::InitParam(int paramId, const std::string& param)
{
    switch (paramId)
    {
    case SCRIPTINGPARAM_BLOCK_JUKEBOX_ITEMLIST:
    {
        std::istringstream allowed_items(param);
        int value;
        for (std::string each; std::getline(allowed_items, each, ',');)
        {
            std::istringstream(each) >> value;
            item_list.insert(value);
        }
        break;
    }
    default:
        AssertSwitchBadDefault(paramId)
        break;
    }
}

} /* namespace Scripting */
