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

Window::Window(i_windowId id, World::EntityPlayer* player, const WindowStaticData* windowData)
    : id(id), player(player), windowData(windowData), offset(0), isPainting(false), paintingButton(0), currentPaintingAction(0)
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

void Window::OpenWindow(int x, i_height y, int z)
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

    OpenWindow(true);
}

void Window::OpenWindow(bool sendOpenPacket)
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

void Window::CloseWindow(bool sendPacket)
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

bool Window::ClickOnWindow(short slotId, char button, short action, char mode, const Inventory::ItemStack* slot)
{
    bool returnValue = false;
    if (slotId >= 0 && slotId < windowData->getMaxSlot())
    {
        if (script)
            script->OnClickOnWindow(player, slotId, button, action, mode, slot);

        i_slot inventorySlotId = 0;
        Inventory::Inventory* inventory = getInventoryForSlot(slotId, inventorySlotId);
        if (inventory == nullptr)
        {
            return false;
        }
        const Inventory::ItemStack* lookedClickedItem = player->GetClickedItem()->LookSlot(0);
        const Inventory::ItemStack* lookedItemInSlot = inventory->LookSlot(inventorySlotId);
        // TODO: switch
        if (mode == WINDOW_CLICK_MODE_CLICK)
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
                    Inventory::ItemStack* itemInSlot = inventory->TakeAndSetSlot(inventorySlotId, clickedItem);
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
        else if (mode == WINDOW_CLICK_MODE_SHIFT)
        {

        }
        else if (mode == WINDOW_CLICK_MODE_KEYBOARD)
        {

        }
        else if (mode == WINDOW_CLICK_MODE_MIDDLE)
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
        else if (mode == WINDOW_CLICK_MODE_DROP)
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
        else if (mode == WINDOW_CLICK_MODE_PAINTING) // "painting"
        {
            // button = 0x0111
            //             ^^^-- action: start, move, end
            //             |----- button: left/righ
            int paintAction = button & 0x3;
            if (paintAction == PAINTING_ACTION_PROGRESS)
            {
                return progressPainting(slotId, action);
            }
            else
            {
                std::cerr << "Bad value: paintAction: " << paintAction << std::endl;
            }
        }
        else if (mode == WINDOW_CLICK_MODE_DOUBLECLICK) // double click
        {
            if (button == 0)
            {
                Inventory::ItemStack* clickedItem =  player->GetClickedItem()->TakeSlot(0);
                if (clickedItem != nullptr)
                {
                    for (Inventory::Inventory* inv : inventoryList)
                    {
                        inv->TakeStackableItemAndFillStack(clickedItem);
                    }
                }
                player->GetClickedItem()->ClearAndSetSlot(0, clickedItem);
            }
        }
        else
        {
            // Handle bad value (kick player + log)
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
        else if (mode == WINDOW_CLICK_MODE_PAINTING) // "painting"
        {
            // button = 0x0111
            //             ^^^-- action: start, move, end
            //             |----- button: left/righ
            int paintAction = button & 0x3;
            int mouseButton = (button >> 2) & 0x3;
            switch (paintAction)
            {
            case PAINTING_ACTION_START:
                return startPainting(mouseButton, action);
            case PAINTING_ACTION_END:
                return endPainting(action);
            default:
                std::cerr << "Bad value: paintAction: " << paintAction << "with slot=-999" << std::endl;
                break;
            }
        }

    }
    player->UpdateInventories();
    UpdateInventories();
    return returnValue;
}

void Window::ConfirmTransaction(short action, bool accepted)
{
    if (script)
        script->OnConfirmTransaction(player, action, accepted);
}

void Window::DoAction(short action)
{
    if (script)
        script->OnDoAction(player, action);
}

void Window::SetSlot(short slotId, const Inventory::ItemStack* slot)
{
    i_slot inventorySlotId = 0;
    Inventory::Inventory* inventory = getInventoryForSlot(slotId, inventorySlotId);
    if (inventory != nullptr)
    {
        if (slot == nullptr)
        {
            inventory->ClearSlot(inventorySlotId);
        }
        else
        {
            inventory->ClearAndSetSlot(inventorySlotId, slot->Copy());
        }
        return;
    }
}

void Window::SetWindowItems(short /*slotId*/, const Inventory::ItemStack* /*slot*/)
{
}

void Window::AddInventory(Inventory::Inventory* inventory)
{
    inventoryList.push_back(inventory);
    inventory->OpenInventory(player, id, offset);
    offset += inventory->GetMaxSlot();
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

bool Window::startPainting(int mouseButton, int action)
{
    if (!isPainting)
    {
        isPainting = true;
        currentPaintingAction = action;
        paintingButton = mouseButton;
        paintedSlot.clear();
        return true;
    }
    return false;
}

bool Window::progressPainting(i_slot slotId, int action)
{
    if (isPainting && action > currentPaintingAction)
    {
        currentPaintingAction = action;
        paintedSlot.insert(slotId);
        return true;
    }
    return false;
}

bool Window::endPainting(int action)
{
    if (isPainting)
    {
        isPainting = false;
        if (action > currentPaintingAction)
        {
            if (!paintedSlot.empty())
            {
                Inventory::Inventory* clickedInventory =  player->GetClickedItem();
                const Inventory::ItemStack* lookClickedItem = clickedInventory->LookSlot(0);
                if (lookClickedItem != nullptr)
                {
                    size_t stackedItem = lookClickedItem->getStackSize();
                    size_t countSlotPainted = paintedSlot.size();
                    size_t itemPerSlot = 1;
                    if (paintingButton == 0)
                        itemPerSlot = stackedItem / countSlotPainted;
                    if (itemPerSlot > 0 && stackedItem >= itemPerSlot * countSlotPainted)
                    {
                        for (i_slot slotId : paintedSlot)
                        {
                            i_slot inventorySlotId = 0;
                            Inventory::Inventory* inventory = getInventoryForSlot(slotId, inventorySlotId);
                            if (inventory == nullptr)
                            {
                                continue;
                            }
                            Inventory::ItemStack* takenItems = clickedInventory->TakeSomeItemInSlot(0, itemPerSlot);
                            Inventory::ItemStack* remainingItems = inventory->Merge(inventorySlotId, takenItems, itemPerSlot);
                            if (remainingItems != nullptr)
                                clickedInventory->Merge(0, remainingItems, -1);
                        }
                        paintedSlot.clear();
                    }
                }
            }
            return true;
        }
    }
    return false;
}

Inventory::Inventory* Window::getInventoryForSlot(i_slot slotId, i_slot& inventorySlotId)
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
    inventorySlotId = slotId - slotOffsetInInventory;
    return inventory;
}

} /* namespace Window */
