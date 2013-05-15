#include "BlockFurnaceScript.h"

#include "Block/TileEntities/TileEntityFurnace.h"
#include "Entity/EntityPlayer.h"
#include "Util/AssertUtil.h"
#include "Window/Window.h"
#include "Window/WindowStaticData.h"
#include "Window/WindowList.h"

namespace Scripting
{

BlockFurnaceScript::BlockFurnaceScript()
    : BlockScript("block_furnace")
    , windowDataId(0)
{
}

BlockFurnaceScript::~BlockFurnaceScript()
{
}

BlockScript* BlockFurnaceScript::Copy()
{
    return new BlockFurnaceScript(*this);
}

void BlockFurnaceScript::InitParam(int paramId, int param)
{
    switch(paramId)
    {
    case SCRIPTINGPARAM_BLOCK_FURNACe_WINDOWID:
        windowDataId = param;
        break;
    default:
        AssertSwitchBadDefault(paramId);
        break;
    }
}

Block::TileEntity* BlockFurnaceScript::CreateNewTileEntity(int blockX, i_height blockY, int blockZ) const
{
    return new Block::TileEntityFurnace(blockX, blockY, blockZ);
}


bool BlockFurnaceScript::UseTileEntity() const
{
    return true;
}

bool BlockFurnaceScript::OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char /*face*/, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    const Window::WindowStaticData* windowData = Window::WindowList::getWindowData(windowDataId);
    if (windowData != nullptr)
    {
        Window::Window* window = new Window::Window(user->GetNextAndSetCurrentWindowId(), windowData);
        window->OpenWindow(user, x, y, z);
    }
    return true;
}

void BlockFurnaceScript::OnBlockPlacedBy(World::EntityPlayer* player, int /*x*/, i_height /*y*/, int /*z*/, int /*face*/, i_block& /*blockId*/, i_data& data, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    int playerRotation = (int)(std::floor((player->getYaw() * 4.0 / 360.0) + 0.5)) & 3;
    data = 0;
    switch (playerRotation)
    {
    case 0:
        data |= 2;
        break;
    case 1:
        data |= 5;
        break;
    case 2:
        data |= 3;
        break;
    case 3:
        data |= 4;
        break;
    default:
        AssertSwitchBadDefault(playerRotation)
        break;
    }
}

// On click open window using id in parameter of script from database
void BlockFurnaceScript::OnDestroy(World::World* /*world*/, int /*x*/, i_height /*y*/, int /*z*/, i_data /*data*/) const
{
    // TODO: Drop item in tile entity
}

} /* namespace Scripting */
