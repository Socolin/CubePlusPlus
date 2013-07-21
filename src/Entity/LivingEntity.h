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
#define MAX_HURTTIME 10

class LivingEntity: public Entity
{
    typedef Entity parent_type;
public:
    LivingEntity(eEntityType entityType, int entityTypeFlag, double x, double y, double z);
    virtual ~LivingEntity();
    virtual float getEyeHeight();

    virtual void UpdateTick() override;
    virtual bool Load(nbt::TagCompound* tagNbtData) override;
    virtual bool Save(nbt::TagCompound* tagNbtData) override;

    virtual void MoveLiving(double dx, double dz);
    virtual short GetMaxHealth();

    void SetCustomName(const std::wstring& customName);
    void SetCustomNameVisible(bool visible);

    virtual void Attack(LivingEntity* attacker, int& damage) override;
    virtual void DealDamage(int damage);

    virtual void GetSpecificUpdatePacket(Network::NetworkPacket& packet) override;


protected:
    bool hasChangeItemInHand;

    bool hasTakeDamage;
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
