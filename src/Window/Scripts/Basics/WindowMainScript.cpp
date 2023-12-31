#include "WindowMainScript.h"

#include "Window/Window.h"
#include "Inventory/InventoryCraft.h"
#include "Entity/EntityPlayer.h"

namespace Scripting
{

WindowMainScript::WindowMainScript()
    : WindowScript("window_main")
{
}

WindowMainScript::~WindowMainScript()
{
}

WindowScript* WindowMainScript::Copy() const
{
    return new WindowMainScript(*this);
}

void WindowMainScript::OnCloseWindow(World::EntityPlayer* player)
{
    Inventory::Inventory* inventory = baseWindow->GetInventoryByType(Inventory::INVENTORY_TYPE_CRAFT);
    if (inventory != nullptr)
        inventory->DropInventory(player);
}

void WindowMainScript::OnPostClickOnWindow(World::EntityPlayer* /*player*/, short /*slotId*/, char /*button*/, short /*action*/, char /*mode*/, const Inventory::ItemStack* /*slot*/)
{
    Inventory::Inventory* inventory = baseWindow->GetInventoryByType(Inventory::INVENTORY_TYPE_CRAFT);
    if (inventory != nullptr)
    {
        Inventory::InventoryCraft* craftInventory = dynamic_cast<Inventory::InventoryCraft*>(inventory);
        if (craftInventory)
            craftInventory->PerformCraftChecking();
    }
}

bool WindowMainScript::OnClickOnWindow(World::EntityPlayer* player, short slotId, char button, short /*action*/, char mode, const Inventory::ItemStack* /*slot*/, bool& retValue)
{
    Inventory::Inventory* cr_inventory = baseWindow->GetInventoryByType(Inventory::INVENTORY_TYPE_CRAFT);
    Inventory::InventoryCraft* craftInventory = nullptr;
    if (cr_inventory != nullptr)
    {
        craftInventory = dynamic_cast<Inventory::InventoryCraft*>(cr_inventory);
        if (!craftInventory)
            return false;
    }

    i_slot inventorySlotId = 0;
    Inventory::Inventory* inventory = baseWindow->GetInventoryForSlot(slotId, inventorySlotId);
    if (inventory->GetInventoryType() == Inventory::INVENTORY_TYPE_CRAFT)
    {
        if (inventorySlotId == craftInventory->GetResultSlotId())
        {
            if (mode == Window::WINDOW_CLICK_MODE_CLICK)
            {
                retValue = true;
                const Inventory::ItemStack* lookedClickedItem = player->GetClickedItem()->LookSlot(0);
                const Inventory::ItemStack* lookedCraftResult = inventory->LookSlot(craftInventory->GetResultSlotId());
                if (lookedCraftResult != nullptr)
                {
                    if (lookedClickedItem == nullptr || (lookedClickedItem->GetMaxStackSize() >= lookedClickedItem->getStackSize() + lookedCraftResult->getStackSize() && lookedClickedItem->IsStackable(lookedCraftResult)))
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
                if (button == 0 || button == 1)
                {
                    const Inventory::ItemStack* lookedCraftResult = inventory->LookSlot(craftInventory->GetResultSlotId());
                    if (lookedCraftResult != nullptr)
                    {
                        Inventory::eInventoryType invType = inventory->GetInventoryType();

                        i_slot targetSlot = 0;
                        bool reverseInventoriesOrder = false;
                        bool reverseSlotOrder = false;
                        int inventoryTypeFlag = GetInventoryAndSlotShiftClickTarget(invType, inventorySlotId, targetSlot, lookedCraftResult, reverseInventoriesOrder, reverseSlotOrder);

                        bool fillEmptySlot = false;
                        int start = 0;
                        int step = 1;

                        int countSpace = baseWindow->CountAvaibleSpaceForItem(inventoryTypeFlag, lookedCraftResult);
                        int countCraft = countSpace / lookedCraftResult->getStackSize();
                        if (countCraft > 0)
                        {
                            Inventory::ItemStack* craftedItem = craftInventory->GetResultItems(countCraft);
                            // TODO : use PlaceAllItemInStackToInventories in window class
                            const std::vector<Inventory::Inventory*>& inventoryListByPriority = baseWindow->GetInventoryListByPriority();
                            if (reverseInventoriesOrder)
                            {
                                start = inventoryListByPriority.size() - 1;
                                step = -1;
                            }
                            // Two pass, first filling all non empty slot, second fill empty slot
                            for (int i = 0; i < 2; i++)
                            {
                                for (size_t invId = start; invId < inventoryListByPriority.size(); invId += step)
                                {
                                    Inventory::Inventory* inv = inventoryListByPriority[invId];

                                    if ((inventoryTypeFlag & inv->GetInventoryType()) != 0)
                                    {
                                        if (craftedItem == nullptr)
                                        {
                                            break;
                                        }
                                        craftedItem = inv->StackStackableItemFromStack(craftedItem, reverseSlotOrder, fillEmptySlot);
                                    }
                                }
                                fillEmptySlot = true;
                            }
                        }
                        retValue = true;
                    }
                }
                return true;
            }
            if (mode == Window::WINDOW_CLICK_MODE_KEYBOARD)
            {
                if (button >= 0 && button < 9)
                {
                    retValue = true;
                    Inventory::Inventory* handInventory = baseWindow->GetInventoryByType(Inventory::INVENTORY_TYPE_PLAYER_HANDS);

                    const Inventory::ItemStack* lookedTargetItem = handInventory->LookSlot(button);
                    const Inventory::ItemStack* lookedCraftResult = inventory->LookSlot(craftInventory->GetResultSlotId());
                    if (lookedCraftResult != nullptr)
                    {
                        if (lookedTargetItem == nullptr || (lookedTargetItem->GetMaxStackSize() >= lookedTargetItem->getStackSize() + lookedCraftResult->getStackSize() && lookedTargetItem->IsStackable(lookedCraftResult)))
                        {
                            Inventory::ItemStack* craftResult = craftInventory->GetResultItems(1);
                            if (craftResult != nullptr)
                            {
                                Inventory::ItemStack* mergeResult = handInventory->Merge(button, craftResult, -1);
                                assert(mergeResult == nullptr);
                            }
                        }
                    }
                    retValue = true;
                }
                return true;
            }
            if (mode == Window::WINDOW_CLICK_MODE_DROP)
            {
                const Inventory::ItemStack* lookedCraftResult = inventory->LookSlot(craftInventory->GetResultSlotId());
                if (lookedCraftResult != nullptr)
                {
                    retValue = true;
                    Inventory::ItemStack* craftResult = craftInventory->GetResultItems(1);
                    if (craftResult != nullptr)
                    {
                        player->DropItem(craftResult);
                    }
                }
                return true;
            }
        }
    }
    return false;
}

int WindowMainScript::GetInventoryAndSlotShiftClickTarget(Inventory::eInventoryType clickedInventoryType, i_slot slotId, i_slot& targetSlot, const Inventory::ItemStack* /*slotItemStack*/, bool& reverseInventoriesOrder, bool& reverseSlotOrder)
{
    if (clickedInventoryType == Inventory::INVENTORY_TYPE_CRAFT)
    {
        reverseInventoriesOrder = (slotId != 0);
        reverseSlotOrder = (slotId == 0);
        targetSlot = -1;
        return (Inventory::INVENTORY_TYPE_PLAYER_HANDS | Inventory::INVENTORY_TYPE_PLAYER_MAIN);
    }
    else if (clickedInventoryType == Inventory::INVENTORY_TYPE_PLAYER_MAIN)
    {
        targetSlot = -1;
        return (Inventory::INVENTORY_TYPE_PLAYER_HANDS);
    }
    else if (clickedInventoryType == Inventory::INVENTORY_TYPE_PLAYER_HANDS)
    {
        targetSlot = -1;
        return (Inventory::INVENTORY_TYPE_PLAYER_MAIN);
    }
    return 0;
}
} /* namespace Scripting */
