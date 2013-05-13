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
   assert(windowData->GetScript());
   script = windowData->GetScript()->Copy();
   assert(script);
   script->Init(this);
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
    script->OnOpenWindow(player);

    player->OpenWindow(this);

    // TODO: move this in player and add getOpenPacket in script or something to rename window
    Network::NetworkPacket openWindowPacket(Network::OP_OPEN_WINDOW);
    openWindowPacket << id << windowData->GetClientWindowId() << windowData->getName() << windowData->getMaxSlot() << true;
    player->Send(openWindowPacket);

    Network::NetworkPacket setWindowItemPacket(Network::OP_SET_WINDOW_ITEMS);
    setWindowItemPacket << id << (short)        windowData->getMaxSlot();
    for (Inventory::Inventory* inv : inventoryList)
    {
        inv->SendInventoryTo(player, setWindowItemPacket);
    }
    player->Send(setWindowItemPacket);
}

void Window::CloseWindow(World::EntityPlayer* player, bool askByPlayer)
{
    script->OnCloseWindow(player);

    for (Inventory::Inventory* inv : inventoryList)
    {
        inv->CloseInventory(player);
    }
    player->GetInventory().OpenInventory(player, i_windowId(9), 0);

    if (!askByPlayer)
    {
        Network::NetworkPacket closeWindowPacket(Network::OP_CLICK_WINDOW);
        closeWindowPacket << id;
        player->Send(closeWindowPacket);
    }
}

bool Window::ClickOnWindow(World::EntityPlayer* player, short slotId, char button, short action, char mode, const Inventory::ItemStack* slot)
{
    if (slotId >= 0 && slotId < windowData->getMaxSlot())
    {
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
//        const Inventory::ItemStack* clickedItem = player->GetClickedItem().LookSlot(0);
//        const Inventory::ItemStack* itemInSlot = inventory->LookSlot(slotId - slotOffsetInInventory);
        if (mode == 0)
        {
            if (button == 0)
            {
                Inventory::ItemStack* clickedItem =  player->GetClickedItem().TakeSlot(0);
                Inventory::ItemStack* itemInSlot = inventory->TakeAndSetSlot(slotId - slotOffsetInInventory, clickedItem);
                player->GetClickedItem().ClearAndSetSlot(0, itemInSlot);
                return true;
            }
        }
    }
    else if (slotId == -999)
    {

    }
    return false;
}

void Window::ConfirmTransaction(World::EntityPlayer* player, short action, bool accepted)
{
}

void Window::DoAction(World::EntityPlayer* player, short action)
{
}

void Window::SetSlot(World::EntityPlayer* player, short slotId, const Inventory::ItemStack* slot)
{
}

void Window::SetWindowItems(World::EntityPlayer* player, short slotId, const Inventory::ItemStack* slot)
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

} /* namespace Window */
