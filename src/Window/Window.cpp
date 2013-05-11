#include "Window.h"

#include <cassert>

#include "Block/Block.h"
#include "Block/BlockList.h"
#include "Script/WindowScript.h"
#include "Entity/EntityPlayer.h"
#include "World/World.h"

namespace Window
{

Window::Window(i_windowId id, Scripting::WindowScript* script, const WindowStaticData& windowData)
    : id(id), script(script), windowData(windowData)
{
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
}

void Window::CloseWindow(World::EntityPlayer* /*player*/)
{
}

void Window::ClickOnWindow(World::EntityPlayer* /*player*/, short /*slotId*/, char /*button*/, short /*action*/, char /*mode*/, const Inventory::ItemStack& /*slot*/)
{
}

void Window::SetSlot(World::EntityPlayer* /*player*/, short /*slotId*/, const Inventory::ItemStack& /*slot*/)
{
}

void Window::SetWindowItems(World::EntityPlayer* /*player*/, short /*slotId*/, const Inventory::ItemStack& /*slot*/)
{
}

void Window::ConfirmTransaction(World::EntityPlayer* /*player*/, short /*action*/, bool /*accepted*/)
{
}

void Window::DoAction(World::EntityPlayer* /*player*/, short /*action*/)
{
}

} /* namespace Window */
