#include "TileEntityFurnace.h"

#include "Database/MiscData/FurnaceRecipes.h"
#include "Inventory/Item.h"
#include "Inventory/InventoryFurnace.h"
#include "Block/Scripts/Basics/BlockFurnaceScript.h"
#include "World/World.h"

namespace Block
{

TileEntityFurnace::TileEntityFurnace(World::World* world, int blockX, i_height blockY, int blockZ)
    : TileEntity(TILEENTITY_TYPE_FURNACE, world, blockX, blockY, blockZ)
    , fuel(0), maxfuel(0), progress(0)
{
    inventory = new Inventory::InventoryFurnace();
}

TileEntityFurnace::~TileEntityFurnace()
{
    inventory->CloseInventoryForDelete();
    delete inventory;
}

void TileEntityFurnace::UpdateTick()
{
    bool burning = fuel > 0;
    if (fuel > 0)
    {
        const Inventory::ItemStack* lookedInputItem = inventory->LookSlot(inventory->GetInputSlotId());
        const Inventory::ItemStack* lookedResultItem = inventory->LookSlot(inventory->GetResultSlotId());
        if (lookedInputItem != nullptr)
        {
            const Database::FurnaceRecipes::Recipe& recipe = Database::FurnaceRecipes::Instance().GetRecipe(lookedInputItem->getItemId(), lookedInputItem->getItemData());
            if (recipe.itemId > 0)
            {
                if (lookedResultItem == nullptr || (!lookedResultItem->Full() && lookedResultItem->IsStackable(recipe.resultId, recipe.resultData)))
                {
                    progress++;
                    if (progress >= 200)
                    {
                        inventory->RemoveSomeItemInSlot(inventory->GetInputSlotId(), 1);
                        Inventory::ItemStack* mergeResult = inventory->Merge(inventory->GetResultSlotId(), new Inventory::ItemStack(recipe.resultId,recipe.resultQtt, recipe.resultData));
                        if (mergeResult != nullptr)
                        {
                            delete mergeResult;
                        }
                        progress = 0;
                        inventory->SendUpdateToAllViewer();
                    }
                }
                else
                {
                    progress = 0;
                }
            }
            else
            {
                progress = 0;
            }
        }
        else
        {
            progress = 0;
        }
        fuel--;
        inventory->UpdateWindowProperty(0, progress);
        inventory->UpdateWindowProperty(1, fuel * 200 / maxfuel);
    }
    if (fuel == 0)
    {
        const Inventory::ItemStack* lookedInputItem = inventory->LookSlot(inventory->GetInputSlotId());
        const Inventory::ItemStack* lookedFuelItem = inventory->LookSlot(inventory->GetFuelSlotId());
        if (lookedFuelItem != nullptr && lookedInputItem != nullptr)
        {
            const Inventory::Item* fuelType = lookedFuelItem->getItem();
            if (fuelType && fuelType->getBurningTime() > 0)
            {
                const Database::FurnaceRecipes::Recipe& recipe = Database::FurnaceRecipes::Instance().GetRecipe(lookedInputItem->getItemId(), lookedInputItem->getItemData());
                if (recipe.itemId > 0)
                {
                    fuel = fuelType->getBurningTime();
                    maxfuel = fuel;
                    if (fuelType->getContainerId())
                    {
                        inventory->ClearAndSetSlot(inventory->GetFuelSlotId(), new Inventory::ItemStack(fuelType->getContainerId(), 1, 0));
                    }
                    else
                    {
                        inventory->RemoveSomeItemInSlot(inventory->GetFuelSlotId(), 1);
                    }
                    inventory->SendUpdateToAllViewer();
                    inventory->UpdateWindowProperty(1, fuel * 200 / maxfuel);
                }
            }
        }
    }
    if (burning && fuel == 0)
    {
        world->NotifyTileEntityStateChange(blockX, blockY, blockZ, Scripting::BlockFurnaceScript::ACTION_STOP_BURNING);
    }
    else if (!burning && fuel > 0)
    {
        world->NotifyTileEntityStateChange(blockX, blockY, blockZ, Scripting::BlockFurnaceScript::ACTION_START_BURNING);
    }
}

bool TileEntityFurnace::NeedUpdate()
{
    return true;
}

void TileEntityFurnace::GetDataPacket(Network::NetworkPacket& /*packet*/)
{
}

bool TileEntityFurnace::HasNetworkData()
{
    return false;
}

Inventory::Inventory* TileEntityFurnace::GetInventory()
{
    return inventory;
}

TileEntity* TileEntityFurnace::Create(World::World* world, int blockX, i_height blockY, int blockZ)
{
    return new TileEntityFurnace(world, blockX, blockY, blockZ);
}

void TileEntityFurnace::Load(nbt::TagCompound* nbtData)
{
    /*FIXME*/
}

void TileEntityFurnace::Save(nbt::TagCompound* nbtData)
{
    /*FIXME*/
}

const char* TileEntityFurnace::GetName()
{
    return "Furnace";
}

} /* namespace Block */
