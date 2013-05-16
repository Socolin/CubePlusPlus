#include "WindowFurnaceScript.h"

#include <cassert>

#include "Block/TileEntities/TileEntity.h"
#include "Entity/EntityPlayer.h"
#include "Window/Window.h"

namespace Scripting
{

WindowFurnaceScript::WindowFurnaceScript()
    : WindowScript("window_furnace")
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
}

} /* namespace Scripting */
