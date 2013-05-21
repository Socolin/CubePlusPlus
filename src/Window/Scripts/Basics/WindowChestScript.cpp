#include "WindowChestScript.h"

#include "Block/TileEntities/TileEntity.h"
#include "Block/TileEntities/TileEntityChest.h"
#include "Database/MiscData/FurnaceRecipes.h"
#include "Entity/EntityPlayer.h"
#include "Inventory/InventoryFurnace.h"
#include "Inventory/Item.h"
#include "Window/Window.h"
#include "World/World.h"

namespace Scripting
{

WindowChestScript::WindowChestScript()
        : WindowScript("window_chest")
        , chestTileEntity(nullptr)
{
}


WindowChestScript::WindowChestScript(std::string scriptName)
    : WindowScript(scriptName.c_str())
    , chestTileEntity(nullptr)
{
}

WindowChestScript::~WindowChestScript()
{
}

WindowScript* WindowChestScript::Copy() const
{
    return new WindowChestScript(*this);
}

void WindowChestScript::OnOpenWindow(World::EntityPlayer* player)
{
    baseWindow->AddInventory(player->GetMainInventory());
    baseWindow->AddInventory(player->GetHandsInventory());
}

void WindowChestScript::OnOpenWindow(World::EntityPlayer* /*player*/, Block::TileEntity* tileEntity)
{
    assert(tileEntity->getType() == Block::TILEENTITY_TYPE_CHEST);
    Inventory::Inventory* chestInventory = tileEntity->GetInventory();
    chestTileEntity = tileEntity;
    tileEntity->NotifyPlayerUse(Block::TileEntityChest::TILEENTITY_PLAYER_OPEN);
    baseWindow->AddInventory(chestInventory);
}

void WindowChestScript::OnCloseWindow(World::EntityPlayer* player)
{
    if (chestTileEntity)
    {
        chestTileEntity->NotifyPlayerUse(Block::TileEntityChest::TILEENTITY_PLAYER_CLOSE);
    }
}

int WindowChestScript::GetInventoryAndSlotShiftClickTarget(Inventory::eInventoryType clickedInventoryType, i_slot /*slotId*/, i_slot& targetSlot, const Inventory::ItemStack* /*slotItemStack*/, bool& reverseOrder)
{
    if (clickedInventoryType == Inventory::INVENTORY_TYPE_DEFAULT)
    {
        reverseOrder = true;
        targetSlot = -1;
        return (Inventory::INVENTORY_TYPE_PLAYER_HANDS | Inventory::INVENTORY_TYPE_PLAYER_MAIN);
    }
    else if (clickedInventoryType == Inventory::INVENTORY_TYPE_PLAYER_MAIN)
    {
        targetSlot = -1;
        return (Inventory::INVENTORY_TYPE_DEFAULT  | Inventory::INVENTORY_TYPE_PLAYER_HANDS);
    }
    else if (clickedInventoryType == Inventory::INVENTORY_TYPE_PLAYER_HANDS)
    {
        targetSlot = -1;
        return (Inventory::INVENTORY_TYPE_DEFAULT  | Inventory::INVENTORY_TYPE_PLAYER_MAIN);
    }
    return 0;
}

} /* namespace Scripting */
