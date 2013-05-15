#include "Window.h"

#include <cassert>

#include "Block/Block.h"
#include "Block/BlockList.h"
#include "Script/WindowScript.h"
#include "Entity/EntityPlayer.h"
#include "Network/NetworkPacket.h"
#include "Network/OpcodeList.h"
#include "World/World.h"

namespace Window
{

Window::Window(i_windowId id, const WindowStaticData* windowData)
    : id(id), windowData(windowData)
{
   assert(windowData);
   if (windowData->GetScript() != nullptr)
   {
       script = windowData->GetScript()->Copy();
       if (script)
           script->Init(this);
   }
}

Window::~Window()
{
    delete script;
}

void Window::OpenWindow(World::EntityPlayer* player, int x, i_height y, int z)
{
    World::World* world = player->getWorld();
    if (world)
    {
        i_block blockId = world->GetBlockId(x, y, z);
        if (blockId)
        {
            const Block::Block* block = Block::BlockList::getBlock(blockId);
            if (block->UseTileEntity())
            {
                Block::TileEntity* tileEntity = world->GetTileEntity(x, y, z);
                if (tileEntity)
                {
                    script->OnOpenWindow(player, tileEntity);
                }
            }
        }
    }

    player->OpenWindow(this);
    if (script)
        script->OnOpenWindow(player);

    OpenWindow(player, true);
}

void Window::OpenWindow(World::EntityPlayer* player, bool sendOpenPacket)
{
    if (sendOpenPacket)
    {
        Network::NetworkPacket openWindowPacket(Network::OP_OPEN_WINDOW);
        openWindowPacket << id << windowData->GetClientWindowId() << windowData->getName() << windowData->getMaxSlot() << true;
        player->Send(openWindowPacket);
    }

    Network::NetworkPacket setWindowItemPacket(Network::OP_SET_WINDOW_ITEMS);
    setWindowItemPacket << id << (short) windowData->getMaxSlot();
    for (Inventory::Inventory* inv : inventoryList)
    {
        inv->SendInventoryTo(setWindowItemPacket);
    }
    player->Send(setWindowItemPacket);
}

void Window::CloseWindow(World::EntityPlayer* player, bool sendPacket)
{
    for (Inventory::Inventory* inv : inventoryList)
    {
        inv->CloseInventory(player);
    }

    if (script)
        script->OnCloseWindow(player);

    if (sendPacket)
    {
        Network::NetworkPacket closeWindowPacket(Network::OP_CLOSE_WINDOW);
        closeWindowPacket << id;
        player->Send(closeWindowPacket);
    }
}

bool Window::ClickOnWindow(World::EntityPlayer* player, short slotId, char button, short action, char mode, const Inventory::ItemStack* slot)
{
    bool returnValue = false;
    if (slotId >= 0 && slotId < windowData->getMaxSlot())
    {
        if (script)
            script->OnClickOnWindow(player, slotId, button, action, mode, slot);

        int slotOffsetInInventory = 0;
        Inventory::Inventory* inventory = nullptr;
        for (Inventory::Inventory* inv : inventoryList)
        {
            int maxSlot = inv->GetMaxSlot();
            if (maxSlot + slotOffsetInInventory > slotId)
            {
                inventory = inv;
                break;
            }
            slotOffsetInInventory += maxSlot;
        }
        if (inventory == nullptr)
        {
            return false;
        }
        i_slot inventorySlotId = slotId - slotOffsetInInventory;
        const Inventory::ItemStack* lookedClickedItem = player->GetClickedItem()->LookSlot(0);
        const Inventory::ItemStack* lookedItemInSlot = inventory->LookSlot(inventorySlotId);
        if (mode == 0)
        {
            if (button == 0)
            {
                Inventory::ItemStack* clickedItem =  player->GetClickedItem()->TakeSlot(0);
                if (lookedClickedItem != nullptr && lookedItemInSlot != nullptr && lookedClickedItem->IsSoftEqual(lookedItemInSlot))
                {
                    Inventory::ItemStack* mergeResult = inventory->Merge(inventorySlotId, clickedItem);
                    player->GetClickedItem()->ClearAndSetSlot(0, mergeResult);
                }
                else
                {
                    Inventory::ItemStack* itemInSlot = inventory->TakeAndSetSlot(slotId - slotOffsetInInventory, clickedItem);
                    player->GetClickedItem()->ClearAndSetSlot(0, itemInSlot);
                }
                returnValue = true;
            }
            else if (button == 1)
            {
                if (lookedClickedItem == nullptr)
                {
                    if (lookedItemInSlot == nullptr)
                    {
                        returnValue = true;
                    }
                    else
                    {
                        int count = lookedItemInSlot->getStackSize();
                        Inventory::ItemStack* halfStackFromSlot = inventory->TakeSomeItemInSlot(inventorySlotId, (count + 1) / 2);
                        player->GetClickedItem()->ClearAndSetSlot(0, halfStackFromSlot);
                        returnValue = true;
                    }
                }
                else
                {
                    Inventory::ItemStack* clickedItem =  player->GetClickedItem()->TakeSlot(0);
                    Inventory::ItemStack* mergeResult = inventory->Merge(inventorySlotId, clickedItem, 1);
                    player->GetClickedItem()->ClearAndSetSlot(0, mergeResult);
                    returnValue = true;
                }
            }
        }
        else if (mode == 1)
        {

        }
        else if (mode == 2)
        {

        }
        else if (mode == 3) // Middle click
        {
            if (player->GetGameMode() == World::EntityPlayer::GAMEMODE_CREATVE)
            {
                if (lookedClickedItem == nullptr && lookedItemInSlot != nullptr)
                {
                    Inventory::ItemStack* duplicatedItem = lookedItemInSlot->Copy();
                    duplicatedItem->setStackSize(lookedItemInSlot->GetMaxStackSize());
                    player->GetClickedItem()->ClearAndSetSlot(0, duplicatedItem);
                    returnValue = true;
                }
            }
        }
        else if (mode == 4)
        {
            if (lookedItemInSlot != nullptr)
            {
                if (button == 0)
                {
                    Inventory::ItemStack* droppedItem = inventory->TakeSomeItemInSlot(inventorySlotId, 1);
                    if (droppedItem != nullptr)
                        player->DropItem(droppedItem);
                    returnValue = true;
                }
                else if (button == 1)
                {
                    Inventory::ItemStack* itemInSlot=  inventory->TakeSlot(inventorySlotId);
                    player->DropItem(itemInSlot);
                    returnValue = true;
                }
            }
        }
    }
    else if (slotId == -999)
    {
        const Inventory::ItemStack* lookedClickedItem = player->GetClickedItem()->LookSlot(0);

        if (mode == 0)
        {
            if (lookedClickedItem != nullptr)
            {
                if (button == 0)
                {
                    Inventory::ItemStack* clickedItem =  player->GetClickedItem()->TakeSlot(0);
                    player->DropItem(clickedItem);
                    returnValue = true;
                }
                else if (button == 1)
                {
                    Inventory::ItemStack* droppedItem = player->GetClickedItem()->TakeSomeItemInSlot(0, 1);
                    if (droppedItem != nullptr)
                        player->DropItem(droppedItem);
                    returnValue = true;
                }
            }
        }
    }
    player->UpdateInventories();
    UpdateInventories();
    return returnValue;
}

void Window::ConfirmTransaction(World::EntityPlayer* player, short action, bool accepted)
{
    if (script)
        script->OnConfirmTransaction(player, action, accepted);
}

void Window::DoAction(World::EntityPlayer* player, short action)
{
    if (script)
        script->OnDoAction(player, action);
}

void Window::SetSlot(World::EntityPlayer* /*player*/, short slotId, const Inventory::ItemStack* slot)
{
    if (slotId >= 0 && slotId < windowData->getMaxSlot())
    {
        int slotOffsetInInventory = 0;
        Inventory::Inventory* inventory = nullptr;
        for (Inventory::Inventory* inv : inventoryList)
        {
            int maxSlot = inv->GetMaxSlot();
            if (maxSlot + slotOffsetInInventory > slotId)
            {
                inventory = inv;
                break;
            }
            slotOffsetInInventory += maxSlot;
        }
        if (inventory == nullptr)
        {
            return;
        }
        i_slot inventorySlotId = slotId - slotOffsetInInventory;
        inventory->ClearAndSetSlot(inventorySlotId, slot->Copy());
    }
}

void Window::SetWindowItems(World::EntityPlayer* /*player*/, short /*slotId*/, const Inventory::ItemStack* /*slot*/)
{
}

void Window::AddInventory(World::EntityPlayer* player, Inventory::Inventory* inventory, int offset)
{
    // maybe TODO: check order in list with offset.
    inventoryList.push_back(inventory);
    inventory->OpenInventory(player, id, offset);
}

i_windowId Window::GetId() const
{
    return id;
}

const WindowStaticData* Window::GetWindowData() const
{
    return windowData;
}

void Window::ReOpenAllInventories(World::EntityPlayer* player)
{
    int offset = 0;
    for (Inventory::Inventory* inv : inventoryList)
    {
        inv->OpenInventory(player, id, offset);
        offset += inv->GetMaxSlot();
    }
}

void Window::UpdateInventories()
{
    for (Inventory::Inventory* inv : inventoryList)
    {
        inv->SendUpdateToAllViewer();
    }
}

} /* namespace Window */
