#include "WindowScript.h"

#include "Entity/EntityPlayer.h"
#include "Scripting/ScriptManager.h"
#include "Window/Window.h"

namespace Scripting
{

WindowScript::WindowScript(const char* scriptName)
    : baseWindow(nullptr)
{
    ScriptManager::GetInstance()->RegisterScript(scriptName, this);
}

WindowScript::~WindowScript()
{
}

void WindowScript::Init(Window::Window* baseWindow)
{
    this->baseWindow = baseWindow;
}

void WindowScript::OnOpenWindow(World::EntityPlayer* /*player*/)
{
}

void WindowScript::OnOpenWindow(World::EntityPlayer* /*player*/, Block::TileEntity* /*tileEntity*/)
{
}

void WindowScript::OnCloseWindow(World::EntityPlayer* /*player*/)
{
}

void WindowScript::OnClickOnWindow(World::EntityPlayer* /*player*/, short /*slotId*/, char /*button*/, short /*action*/, char /*mode*/, const Inventory::ItemStack* /*slot*/)
{
}

void WindowScript::OnConfirmTransaction(World::EntityPlayer* /*player*/, short /*action*/, bool /*accepted*/)
{
}

void WindowScript::OnDoAction(World::EntityPlayer* /*player*/, short /*action*/)
{
}

int WindowScript::GetInventoryAndSlotShiftClickTarget(Inventory::eInventoryType clickedInventoryType, i_slot slotId, i_slot& targetSlot, const Inventory::ItemStack* slotItemStack, bool& reverseOrder)
{
    if (clickedInventoryType == Inventory::INVENTORY_TYPE_PLAYER_MAIN)
    {
        reverseOrder = false;
        targetSlot = -1;
        return Inventory::INVENTORY_TYPE_PLAYER_HANDS;
    }
    else if (clickedInventoryType == Inventory::INVENTORY_TYPE_PLAYER_HANDS)
    {
        reverseOrder = false;
        targetSlot = -1;
        return Inventory::INVENTORY_TYPE_PLAYER_MAIN;
    }
    return 0;
}

} /* namespace Scripting */
