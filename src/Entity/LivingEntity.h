#ifndef LIVINGENTITY_H_
#define LIVINGENTITY_H_

#include "Entity.h"

namespace Inventory
{
class InventoryEntityEquipement;
}
namespace World
{

#define DEFAULT_MAX_HEALTH 20

class LivingEntity: public Entity
{
    typedef Entity parent_type;
public:
    LivingEntity(eEntityType entityType, int entityTypeFlag, double x, double y, double z);
    virtual ~LivingEntity();
    virtual float getEyeHeight();

    virtual bool Load(nbt::TagCompound* tagNbtData) override;
    virtual bool Save(nbt::TagCompound* tagNbtData) override;

    virtual short GetMaxHealth();

    void SetCustomName(const std::wstring& customName);
    void SetCustomNameVisible(bool visible);
protected:
    bool hasChangeItemInHand;

    short health;
    short hurtTime;
    short deathTime;
    short attackTime;
    bool canPickUpLoot;
    bool persistenceRequired;
    Inventory::InventoryEntityEquipement* equipementInventory;
};

} /* namespace World */
#endif /* LIVINGENTITY_H_ */
