#include "InventoryEntityEquipement.h"

#include "Util/FloatUtil.h"
#include "World/World.h"

namespace Inventory
{

InventoryEntityEquipement::InventoryEntityEquipement()
    : Inventory(INVENTORY_ENTITY_EQUIPEMENT_SIZE)
{
    for (int i = 0; i < INVENTORY_ENTITY_EQUIPEMENT_SIZE; i++)
    {
        dropChance[i] = 0.085f;
    }
}

InventoryEntityEquipement::~InventoryEntityEquipement()
{
}

void InventoryEntityEquipement::SetDropChance(i_slot slotId, float chance)
{
    if (slotId > 0 && slotId < INVENTORY_ENTITY_EQUIPEMENT_SIZE)
        dropChance[slotId] = chance;
}

void InventoryEntityEquipement::DropInventory(World::World* world, double x, double y, double z)
{
    for (size_t i = 0; i < slot.size(); i++)
    {
        ItemStack* item  = slot[i];
        if (item != nullptr)
        {
            if (Util::randFloat() - 0.1f < dropChance[i])
            {
                world->DropItemstackWithRandomDirection(x, y, z, item);
                slot[i] = nullptr;
            }
        }
    }

}

} /* namespace Inventory */
