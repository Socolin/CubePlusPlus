#include "WindowFurnaceScript.h"

#include <cassert>

#include "Block/TileEntities/TileEntity.h"
#include "Database/MiscData/FurnaceRecipes.h"
#include "Entity/EntityPlayer.h"
#include "Inventory/InventoryFurnace.h"
#include "Inventory/Item.h"
#include "Window/Window.h"

namespace Scripting
{

WindowFurnaceScript::WindowFurnaceScript()
    : WindowScript("window_furnace")
    , furnaceInventory(nullptr)
{
}

WindowFurnaceScript::~WindowFurnaceScript()
{
}

WindowScript* WindowFurnaceScript::Copy() const
{
    return new WindowFurnaceScript(*this);
}

void WindowFurnaceScript::OnOpenWindow(World::EntityPlayer* player)
{
    baseWindow->AddInventory(player->GetMainInventory());
    baseWindow->AddInventory(player->GetHandsInventory());
}

void WindowFurnaceScript::OnOpenWindow(World::EntityPlayer* /*player*/, Block::TileEntity* tileEntity)
{
    assert(tileEntity->getType() == Block::TILEENTITY_TYPE_FURNACE);
    Inventory::Inventory* furnaceInventory = tileEntity->GetInventory();
    baseWindow->AddInventory(furnaceInventory);
    this->furnaceInventory = dynamic_cast<Inventory::InventoryFurnace*>(furnaceInventory);
}

int WindowFurnaceScript::GetInventoryAndSlotShiftClickTarget(Inventory::eInventoryType clickedInventoryType, i_slot slotId, i_slot& targetSlot, const Inventory::ItemStack* slotItemStack, bool& reverseOrder)
{
    if (furnaceInventory == nullptr)
    {
        return parent_type::GetInventoryAndSlotShiftClickTarget(clickedInventoryType, slotId, targetSlot, slotItemStack, reverseOrder);
    }
    if (clickedInventoryType == Inventory::INVENTORY_TYPE_FURNACE)
    {
        reverseOrder = (slotId == furnaceInventory->GetResultSlotId());
        targetSlot = -1;
        return (Inventory::INVENTORY_TYPE_PLAYER_HANDS | Inventory::INVENTORY_TYPE_PLAYER_MAIN);
    }
    else if (clickedInventoryType == Inventory::INVENTORY_TYPE_PLAYER_MAIN)
    {
        const Inventory::Item* item = slotItemStack->getItem();
        reverseOrder = false;
        targetSlot = -1;
        if (item && item->getBurningTime())
        {
            targetSlot = furnaceInventory->GetFuelSlotId();
            return Inventory::INVENTORY_TYPE_FURNACE;
        }
        if (Database::FurnaceRecipes::Instance().IsValidInput(slotItemStack))
        {
            targetSlot = furnaceInventory->GetInputSlotId();
            return Inventory::INVENTORY_TYPE_FURNACE;
        }
        return Inventory::INVENTORY_TYPE_PLAYER_HANDS;
    }
    else if (clickedInventoryType == Inventory::INVENTORY_TYPE_PLAYER_HANDS)
    {
        const Inventory::Item* item = slotItemStack->getItem();
        reverseOrder = false;
        targetSlot = -1;
        if (item && item->getBurningTime())
        {
            targetSlot = furnaceInventory->GetFuelSlotId();
            return Inventory::INVENTORY_TYPE_FURNACE;
        }
        if (Database::FurnaceRecipes::Instance().IsValidInput(slotItemStack))
        {
            targetSlot = furnaceInventory->GetInputSlotId();
            return Inventory::INVENTORY_TYPE_FURNACE;
        }
        return Inventory::INVENTORY_TYPE_PLAYER_MAIN;
    }
    return 0;
}

} /* namespace Scripting */
