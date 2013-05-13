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

void WindowScript::OnOpenWindow(World::EntityPlayer* player)
{
}

void WindowScript::OnOpenWindow(World::EntityPlayer* /*player*/, Block::TileEntity* /*tileEntity*/)
{
}

void WindowScript::OnCloseWindow(World::EntityPlayer* player)
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

} /* namespace Scripting */
