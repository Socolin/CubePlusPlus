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
#define DEFAULT_LIVINGSOUND_INTERVAL 80

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
    virtual void SetEntityEat();
    virtual void Kill() override;

    void KnockBack(double dx, double dz);
    virtual void GetSpecificUpdatePacket(Network::NetworkPacket& packet) override;

    // Sound
    float GetSoundVolume() const;
    float GetSoundModifier() const;

    void PlaySound(const std::wstring& sound);
    void PlayLivingSound();
    void PlayHurtSound();
    void PlayDeathSound();

    void SetDeathSound(const std::wstring& deathSound);
    void SetHurtSound(const std::wstring& hurtSound);
    void SetLivingSound(const std::wstring& livingSound);
    void SetLivingSoundInterval(int livingSoundInterval);

protected:
    bool hasChangeItemInHand;

    bool hasTakeDamage;
    bool entityEat;
    short health;
    short hurtTime;
    short deathTime;
    short attackTime;
    bool canPickUpLoot;
    bool persistenceRequired;
    Inventory::InventoryEntityEquipement* equipementInventory;

    int livingSoundTimer;
    int livingSoundInterval;
    std::wstring livingSound;
    std::wstring hurtSound;
    std::wstring deathSound;
};

} /* namespace World */
#endif /* LIVINGENTITY_H_ */
