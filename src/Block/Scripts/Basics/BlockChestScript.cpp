#include "BlockChestScript.h"

#include "Block/BlockConstants.h"
#include "Block/TileEntities/TileEntityChest.h"
#include "Entity/EntityPlayer.h"
#include "Util/AssertUtil.h"
#include "World/World.h"
#include "Window/Window.h"
#include "Window/WindowStaticData.h"
#include "Window/WindowList.h"

namespace Scripting
{

BlockChestScript::BlockChestScript()
        : BlockScript("block_chest"), windowChestDataId(0), windowDoubleChestDataId(0)
{
}

BlockChestScript::~BlockChestScript()
{
}

BlockScript* BlockChestScript::Copy()
{
    return new BlockChestScript(*this);
}

void BlockChestScript::InitParam(int paramId, int param)
{
    switch (paramId)
    {
    case SCRIPTINGPARAM_BLOCK_CHEST_CHESTWINDOWID:
        windowChestDataId = param;
        break;
    case SCRIPTINGPARAM_BLOCK_CHEST_DOUBLECHESTWINDOWID:
        windowDoubleChestDataId = param;
        break;
    default:
        AssertSwitchBadDefault(paramId)
        ;
        break;
    }
}

bool BlockChestScript::CanPlace(World::World* world, int x, i_height y, int z, char /*face*/) const
{
    /*
     * Maybe it can be optimize, in worst cas we test all this case (number is time that we do getBlockId
     *      x x 1 x x
     *      x 2 1 2 x
     *      1 1 4 1 1
     *      x 2 1 2 x
     *      x x 1 x x
     */
    int countChest = 0;
    int doubleChest = 0;

    i_block baseBlockId = baseBlock->GetBlockId();
    FOR_EACH_SIDE_XZ(x, z, blockSide)
        i_block blockId = world->GetBlockId(blockSideX, y, blockSideZ);
        if (blockId == baseBlockId)
        {
            countChest++;
            FOR_EACH_SIDE_XZ(blockSideX, blockSideZ, blockSide)
                i_block blockId = world->GetBlockId(blockSideX, y, blockSideZ);
                if (blockId == baseBlockId)
                {
                    countChest--;
                    doubleChest++;
                    break;
                }END_FOR_EACH_SIDE
        }
        // Check double chest
    END_FOR_EACH_SIDE
    return doubleChest == 0 && countChest < 2;
}

void BlockChestScript::OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int /*face*/, i_block& /*blockId*/, i_data& data, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    World::World* world = player->getWorld();
    int playerRotation = (int)(std::floor((player->getYaw() * 4.0 / 360.0) + 0.5)) & 3;

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

    i_block northBlockId = world->GetBlockId(x, y, z - 1);
    i_block southBlockId = world->GetBlockId(x, y, z + 1);
    i_block westBlockId = world->GetBlockId(x - 1, y, z);
    i_block eastBlockId = world->GetBlockId(x + 1, y, z);

    i_block baseBlockId = baseBlock->GetBlockId();

    if (northBlockId == baseBlockId || southBlockId == baseBlockId || westBlockId == baseBlockId || eastBlockId == baseBlockId)
    {
        if ((northBlockId == baseBlockId || southBlockId == baseBlockId))
        {
            if (data == 4 || data == 5)
            {
                if (northBlockId == baseBlockId)
                {
                    world->ChangeDataNoEvent(x, y, z - 1, data);
                }
                else
                {
                    world->ChangeDataNoEvent(x, y, z + 1, data);
                }
            }
            else
            {
                if (northBlockId == baseBlockId)
                {
                    i_data chestData = world->GetBlockData(x , y, z - 1);
                    if (chestData == 4 || chestData == 5)
                    {
                        data = chestData;
                    }
                    else
                    {
                        data = 5;
                        world->ChangeDataNoEvent(x, y, z - 1, data);
                    }
                }
                else
                {
                    i_data chestData = world->GetBlockData(x, y, z + 1);
                    if (chestData == 4 || chestData == 5)
                    {
                        data = chestData;
                    }
                    else
                    {
                        data = 4;
                        world->ChangeDataNoEvent(x, y, z + 1, data);
                    }
                }
            }
        }

        if ((westBlockId == baseBlockId || eastBlockId == baseBlockId))
        {
            if (data == 2 || data == 3)
            {
                if (westBlockId == baseBlockId)
                {
                    world->ChangeDataNoEvent(x - 1, y, z, data);
                }
                else
                {
                    world->ChangeDataNoEvent(x + 1, y, z, data);
                }
            }
            else
            {
                if (westBlockId == baseBlockId)
                {
                    i_data chestData = world->GetBlockData(x - 1, y, z);
                    if (chestData == 2 || chestData == 3)
                    {
                        data = chestData;
                    }
                    else
                    {
                        data = 2;
                        world->ChangeDataNoEvent(x - 1, y, z, data);
                    }
                }
                else
                {
                    i_data chestData = world->GetBlockData(x + 1, y, z);
                    if (chestData == 2 || chestData == 3)
                    {
                        data = chestData;
                    }
                    else
                    {
                        data = 3;
                        world->ChangeDataNoEvent(x + 1, y, z, data);
                    }
                }
            }
        }
    }
}

bool BlockChestScript::UseTileEntity() const
{
    return true;
}

bool BlockChestScript::OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char /*face*/, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    World::World* world = user->getWorld();
    bool doubleChest = false;
    i_block baseBlockId = baseBlock->GetBlockId();
    FOR_EACH_SIDE_XZ(x, z, blockSide)
        i_block blockId = world->GetBlockId(blockSideX, y, blockSideZ);
        if (blockId == baseBlockId)
        {
            doubleChest = true;
            break;
        }
        // Check double chest
    END_FOR_EACH_SIDE
    const Window::WindowStaticData* windowData = Window::WindowList::getWindowData(doubleChest ? windowDoubleChestDataId : windowChestDataId);
    if (windowData != nullptr)
    {
        Window::Window* window = new Window::Window(user->GetNextAndSetCurrentWindowId(), user, windowData);
        window->OpenWindow(x, y, z);
    }
    return true;
}

Block::TileEntity* BlockChestScript::CreateNewTileEntity(World::World* world, int blockX, i_height blockY, int blockZ) const
{
    return new Block::TileEntityChest(world, blockX, blockY, blockZ);
}

void BlockChestScript::OnDestroy(World::World* world, int x, i_height y, int z, i_data /*data*/) const
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

} /* namespace Scripting */
