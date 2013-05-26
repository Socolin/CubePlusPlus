#include "WindowEnderChestScript.h"

#include "Block/TileEntities/TileEntityEnderChest.h"
#include "Entity/EntityPlayer.h"
#include "Window/Window.h"

namespace Scripting
{

WindowEnderChestScript::WindowEnderChestScript()
    : WindowScript("window_enderchest")
    , enderChestTileEntity(nullptr)
{
}

WindowEnderChestScript::~WindowEnderChestScript()
{
}

WindowScript* WindowEnderChestScript::Copy() const
{
    return new WindowEnderChestScript(*this);
}

void WindowEnderChestScript::OnOpenWindow(World::EntityPlayer* player)
{
    baseWindow->AddInventory(player->GetEnderChestInventory());
    baseWindow->AddInventory(player->GetMainInventory());
    baseWindow->AddInventory(player->GetHandsInventory());
}

void WindowEnderChestScript::OnOpenWindow(World::EntityPlayer* /*player*/, Block::TileEntity* tileEntity)
{
    enderChestTileEntity = tileEntity;
    enderChestTileEntity->NotifyPlayerUse(Block::TileEntityEnderChest::TILEENTITY_PLAYER_OPEN);
}

void WindowEnderChestScript::OnCloseWindow(World::EntityPlayer* /*player*/)
{
    enderChestTileEntity->NotifyPlayerUse(Block::TileEntityEnderChest::TILEENTITY_PLAYER_CLOSE);
}

int WindowEnderChestScript::GetInventoryAndSlotShiftClickTarget(Inventory::eInventoryType clickedInventoryType, i_slot /*slotId*/, i_slot& targetSlot, const Inventory::ItemStack* /*slotItemStack*/, bool& reverseOrder)
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
    return 0;}

} /* namespace Scripting */
