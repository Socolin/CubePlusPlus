#ifndef INVENTORYENTITYEQUIPEMENT_H_
#define INVENTORYENTITYEQUIPEMENT_H_

#include "Inventory.h"

namespace Inventory
{

#define INVENTORY_ENTITY_EQUIPEMENT_SIZE 5
/**
 * Inventory for mob equipement
 */
class InventoryEntityEquipement : public Inventory
{
public:
    InventoryEntityEquipement();
    virtual ~InventoryEntityEquipement();

    void SetDropChance(i_slot slotId, float chance);
    virtual void DropInventory(World::World* world, double x, double y, double z);

private:
    float dropChance[INVENTORY_ENTITY_EQUIPEMENT_SIZE];
};

} /* namespace Inventory */
#endif /* INVENTORYENTITYEQUIPEMENT_H_ */
