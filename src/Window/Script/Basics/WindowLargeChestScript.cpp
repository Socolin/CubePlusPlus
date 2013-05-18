#include "WindowLargeChestScript.h"

#include "Block/TileEntities/TileEntityChest.h"
#include "Window/Window.h"

namespace Scripting
{

WindowLargeChestScript::WindowLargeChestScript()
    : WindowChestScript("window_largechest")
{
}

WindowLargeChestScript::~WindowLargeChestScript()
{
}

WindowScript* WindowLargeChestScript::Copy() const
{
    return new WindowLargeChestScript(*this);
}

void WindowLargeChestScript::OnOpenWindow(World::EntityPlayer* player, Block::TileEntity* tileEntity)
{
    parent_type::OnOpenWindow(player, tileEntity);
    Block::TileEntityChest* tileEntityChest = dynamic_cast<Block::TileEntityChest*>(tileEntity);
    if (tileEntityChest)
    {
        Inventory::Inventory* largeChestInventory = tileEntityChest->GetSecondInventory();
        if (largeChestInventory)
            baseWindow->AddInventory(largeChestInventory);
    }
}

} /* namespace Scripting */
