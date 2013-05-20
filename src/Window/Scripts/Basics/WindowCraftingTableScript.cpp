#include "WindowCraftingTableScript.h"

#include <cassert>

#include "Inventory/InventoryCraft.h"
#include "Entity/EntityPlayer.h"
#include "Window/Window.h"

namespace Scripting
{

WindowCraftingTableScript::WindowCraftingTableScript()
    : WindowScript("window_craftingtable")
    , craftInventory(nullptr)
{
}

WindowCraftingTableScript::~WindowCraftingTableScript()
{
    if (craftInventory)
        craftInventory->CloseInventoryForDelete();
    delete craftInventory;
}

WindowScript* WindowCraftingTableScript::Copy() const
{
    return new WindowCraftingTableScript(*this);
}

void WindowCraftingTableScript::OnOpenWindow(World::EntityPlayer* player)
{
    if (craftInventory == nullptr)
        craftInventory = new Inventory::InventoryCraft(3, 3);
    baseWindow->AddInventory(craftInventory);
    baseWindow->AddInventory(player->GetMainInventory());
    baseWindow->AddInventory(player->GetHandsInventory());
}

void WindowCraftingTableScript::OnCloseWindow(World::EntityPlayer* player)
{
    //TODO: craftInventory drop(player);
    // TODO overide drop inventory for crafting part, don't drop stack in result
}

bool WindowCraftingTableScript::OnClickOnWindow(World::EntityPlayer* player, short slotId, char /*button*/, short /*action*/, char mode, const Inventory::ItemStack* /*slot*/, bool& retValue)
{
    i_slot inventorySlotId = 0;
    Inventory::Inventory* inventory = baseWindow->GetInventoryForSlot(slotId, inventorySlotId);
    if (inventory->GetInventoryType() == Inventory::INVENTORY_TYPE_DEFAULT)
    {
        //TODO: if inventorySlotId == result slot
        if (inventorySlotId == craftInventory->GetResultSlotId())
        {
            if (mode == Window::WINDOW_CLICK_MODE_CLICK)
            {
                retValue = true;
                const Inventory::ItemStack* lookedClickedItem = player->GetClickedItem()->LookSlot(0);
                const Inventory::ItemStack* lookedCraftResult = inventory->LookSlot(craftInventory->GetResultSlotId());
                if (lookedCraftResult != nullptr)
                {
                    if (lookedClickedItem == nullptr || (lookedClickedItem->GetMaxStackSize() >= (int)lookedClickedItem->getStackSize() + (int)lookedCraftResult->getStackSize() && lookedClickedItem->IsStackable(lookedCraftResult)))
                    {
                        Inventory::ItemStack* craftResult = craftInventory->GetResultItems(1);
                        if (craftResult != nullptr)
                        {
                            Inventory::ItemStack* mergeResult = player->GetClickedItem()->Merge(0, craftResult, -1);
                            assert(mergeResult == nullptr);
                        }
                    }
                }
                return true;
            }
            if (mode == Window::WINDOW_CLICK_MODE_SHIFT)
            {
                retValue = true;
                return true;
            }
            if (mode == Window::WINDOW_CLICK_MODE_KEYBOARD)
            {
                retValue = true;
                return true;
            }
            if (mode == Window::WINDOW_CLICK_MODE_MIDDLE)
            {
                retValue = true;
                return true;
            }
            if (mode == Window::WINDOW_CLICK_MODE_DROP)
            {
                retValue = true;
                return true;
            }
        }
    }
    return false;
}

void WindowCraftingTableScript::OnPostClickOnWindow(World::EntityPlayer* player, short slotId, char button, short action, char mode, const Inventory::ItemStack* slot)
{
    craftInventory->PerformCraftChecking();
}

int WindowCraftingTableScript::GetInventoryAndSlotShiftClickTarget(Inventory::eInventoryType clickedInventoryType, i_slot slotId, i_slot& targetSlot, const Inventory::ItemStack* slotItemStack, bool& reverseOrder)
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
