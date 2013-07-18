#include "BlockEnderChestScript.h"

#include "Block/BlockConstants.h"
#include "Block/TileEntities/TileEntityEnderChest.h"
#include "Entity/EntityPlayer.h"
#include "Util/AssertUtil.h"
#include "World/World.h"
#include "Window/Window.h"
#include "Window/WindowStaticData.h"
#include "Window/WindowList.h"

namespace Scripting
{

BlockEnderChestScript::BlockEnderChestScript()
    : BlockScript("block_enderchest")
    , windowEnderChestChestDataId(0)
{
}

BlockEnderChestScript::~BlockEnderChestScript()
{
}

BlockScript* BlockEnderChestScript::Copy()
{
    return new BlockEnderChestScript(*this);
}

void BlockEnderChestScript::InitParam(int paramId, int param)
{
    switch (paramId)
    {
    case SCRIPTINGPARAM_BLOCK_ENDERCHEST_WINDOWENDERCHESTCHESTDATAID:
        windowEnderChestChestDataId = param;
        break;
    default:
        AssertSwitchBadDefault(paramId)
        ;
        break;
    }
}

void BlockEnderChestScript::OnBlockPlacedBy(World::EntityPlayer* player, int /*x*/, i_height /*y*/, int /*z*/, int /*face*/, i_block& /*blockId*/, i_data& data, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    int playerRotation = (int)(std::floor((player->GetYaw() * 4.0 / 360.0) + 0.5)) & 3;

    switch (playerRotation)
    {
    case 0:
        data = 2;
        break;
    case 1:
        data = 5;
        break;
    case 2:
        data = 3;
        break;
    case 3:
        data = 4;
        break;
    default:
        AssertSwitchBadDefault(playerRotation)
        break;
    }
}

bool BlockEnderChestScript::OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char /*face*/, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    const Window::WindowStaticData* windowData = Window::WindowList::getWindowData(windowEnderChestChestDataId);
    if (windowData != nullptr)
    {
        Window::Window* window = new Window::Window(user->GetNextAndSetCurrentWindowId(), user, windowData);
        window->OpenWindow(x, y, z);
    }
    return true;
}

Block::TileEntity* BlockEnderChestScript::CreateNewTileEntity(World::World* world, int blockX, i_height blockY, int blockZ) const
{
    return new Block::TileEntityEnderChest(world, blockX, blockY, blockZ);
}

bool BlockEnderChestScript::UseTileEntity() const
{
    return true;
}

} /* namespace Scripting */
