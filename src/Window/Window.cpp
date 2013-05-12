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
}

void Window::CloseWindow(World::EntityPlayer* player, bool askByPlayer)
{
    script->OnCloseWindow(player);

    for (Inventory::Inventory* inv : inventoryList)
    {
        inv->CloseInventory(player);
    }
    player->GetInventory().OpenInventory(player, 0);

    if (!askByPlayer)
    {
        Network::NetworkPacket closeWindowPacket(Network::OP_CLICK_WINDOW);
        closeWindowPacket << id;
        player->Send(closeWindowPacket);
    }
}

void Window::ClickOnWindow(World::EntityPlayer* player, short slotId, char button, short action, char mode, const Inventory::ItemStack& slot)
{
    if (slotId < windowData->getMaxSlot())
    {
        script->OnClickOnWindow(player, slotId, button, action, mode, slot);

        // lot of thing to do here
    }
}

void Window::ConfirmTransaction(World::EntityPlayer* player, short action, bool accepted)
{
}

void Window::DoAction(World::EntityPlayer* player, short action)
{
}

void Window::SetSlot(World::EntityPlayer* player, short slotId, const Inventory::ItemStack& slot)
{
}

void Window::SetWindowItems(World::EntityPlayer* player, short slotId, const Inventory::ItemStack& slot)
{
}

void Window::AddInventory(World::EntityPlayer* player, Inventory::Inventory* inventory, int offset)
{
    // maybe TODO: check order in list with offset.
    inventoryList.push_back(inventory);
    inventory->OpenInventory(player, offset);
}

const i_windowId Window::GetId() const
{
    return id;
}

const WindowStaticData* Window::GetWindowData() const
{
    return windowData;
}

} /* namespace Window */
