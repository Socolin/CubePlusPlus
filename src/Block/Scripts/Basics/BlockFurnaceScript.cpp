#include "BlockFurnaceScript.h"

#include "Block/TileEntities/TileEntityFurnace.h"
#include "Entity/EntityPlayer.h"
#include "Util/AssertUtil.h"
#include "World/Chunk.h"
#include "World/World.h"
#include "Window/Window.h"
#include "Window/WindowStaticData.h"
#include "Window/WindowList.h"

namespace Scripting
{

BlockFurnaceScript::BlockFurnaceScript()
    : BlockScript("block_furnace")
    , windowDataId(0)
    , idleFurnaceBlock(0)
    , burningFurnaceBlock(0)
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
    case SCRIPTINGPARAM_BLOCK_FURNACE_WINDOWID:
        windowDataId = param;
        break;
    case SCRIPTINGPARAM_BLOCK_FURNACE_IDLEFURNACEBLOCK:
        idleFurnaceBlock = param;
        break;
    case SCRIPTINGPARAM_BLOCK_FURNACE_BURNINGFURNACEBLOCK:
        burningFurnaceBlock = param;
        break;
    default:
        AssertSwitchBadDefault(paramId);
        break;
    }
}

Block::TileEntity* BlockFurnaceScript::CreateNewTileEntity(World::World* world, int blockX, i_height blockY, int blockZ) const
{
    return new Block::TileEntityFurnace(world, blockX, blockY, blockZ);
}


bool BlockFurnaceScript::UseTileEntity() const
{
    return true;
}

ItemUseResult BlockFurnaceScript::OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char /*face*/, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    const Window::WindowStaticData* windowData = Window::WindowList::getWindowData(windowDataId);
    if (windowData != nullptr)
    {
        Window::Window* window = new Window::Window(user->GetNextAndSetCurrentWindowId(), user, windowData);
        window->OpenWindow(x, y, z);
    }
    return ItemUseResult{true, false, 0};
}

void BlockFurnaceScript::OnBlockPlacedBy(World::EntityPlayer* player, int /*x*/, i_height /*y*/, int /*z*/, int /*face*/, i_block& /*blockId*/, i_data& data, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    int playerRotation = (int)(std::floor((player->GetYaw() * 4.0 / 360.0) + 0.5)) & 3;
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

void BlockFurnaceScript::OnDestroy(World::World* world, int x, i_height y, int z, i_data /*data*/) const
{
    Block::TileEntity* tileEntity = world->GetTileEntity(x, y, z);
    if (tileEntity != nullptr)
    {
        Inventory::Inventory* inventory = tileEntity->GetInventory();
        if (inventory != nullptr)
        {
            inventory->DropInventory(world, x, y, z);
        }
    }
}

void BlockFurnaceScript::OnNotifyTileEntityStateChange(World::World* world, int x, i_height y, int z, int action)
{
    switch (action)
    {
    case ACTION_START_BURNING:
        if (baseBlock->GetBlockId() == idleFurnaceBlock)
        {
            World::Chunk* chunk = world->GetChunkIfLoaded(x >> 4, z >> 4);
            if (chunk)
            {
                chunk->ChangeBlockNoEventNoTileEntityChange_DoNotUseExceptIfYouKnowWhatYouDo(x & 0xf, y, z & 0xf, burningFurnaceBlock, chunk->getDataAt(x & 0xf, y, z & 0xf));
            }
        }
        break;
    case ACTION_STOP_BURNING:
        if (baseBlock->GetBlockId() == burningFurnaceBlock)
        {
            World::Chunk* chunk = world->GetChunkIfLoaded(x >> 4, z >> 4);
            if (chunk)
            {
                chunk->ChangeBlockNoEventNoTileEntityChange_DoNotUseExceptIfYouKnowWhatYouDo(x & 0xf, y, z & 0xf, idleFurnaceBlock, chunk->getDataAt(x & 0xf, y, z & 0xf));
            }
        }
        break;
    default:
        break;
    }
}

} /* namespace Scripting */
