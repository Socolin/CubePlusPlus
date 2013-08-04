#include "LivingEntity.h"

#include <cppnbt.h>

#include "Util/StringUtil.h"
#include "Inventory/InventoryEntityEquipement.h"
#include "Network/NetworkPacket.h"
#include "Network/OpcodeList.h"
#include "Util/FloatUtil.h"
#include "World/World.h"

namespace World
{

LivingEntity::LivingEntity(eEntityType entityType, int entityTypeFlag, double x, double y, double z)
    : Entity(entityType, entityTypeFlag | ENTITY_TYPEFLAG_LIVING_ENTITY, x, y, z)
	, hasChangeItemInHand(false)
	, hasTakeDamage(false)
	, entityEat(false)
	, health(DEFAULT_MAX_HEALTH)
	, hurtTime(0)
	, deathTime(0)
	, attackTime(0)
	, canPickUpLoot(false)
	, persistenceRequired(false)
	, equipementInventory(nullptr)
	, livingSoundTimer(0)
	, livingSoundInterval(DEFAULT_LIVINGSOUND_INTERVAL)
	, hurtSound(L"damage.hit")
	, deathSound(L"damage.hit")
{
    equipementInventory = new Inventory::InventoryEntityEquipement();
}

LivingEntity::~LivingEntity()
{
    equipementInventory->CloseInventoryForDelete();
    delete equipementInventory;
}

float LivingEntity::getEyeHeight()
{
    return 1.62f;
}

bool LivingEntity::Load(nbt::TagCompound* tagNbtData)
{
    bool loadSucess = parent_type::Load(tagNbtData);
    if (tagNbtData->hasKey("Health"))
        health = tagNbtData->getShort("Health");
    else
        health = GetMaxHealth();

    hurtTime = tagNbtData->getShort("HurtTime");
    deathTime = tagNbtData->getShort("DeathTime");
    attackTime = tagNbtData->getShort("AttackTime");
    canPickUpLoot = tagNbtData->getBool("CanPickUpLoot");
    persistenceRequired = tagNbtData->getBool("PersistenceRequired");

    if (tagNbtData->hasKey("CustomName"))
    {
//        const std::string& customName = tagNbtData->getString("CustomName");
//        std::wstring customWName;
//        Util::StringToWString(customWName, customName);
//        SetCustomName(customWName);
    }

    nbt::TagList* tagEquipement = tagNbtData->getValueAt<nbt::TagList>("Equipment");
    if (tagEquipement)
    {
        equipementInventory->Load(tagEquipement);
    }

    nbt::TagList* tagActiveEffects = tagNbtData->getValueAt<nbt::TagList>("ActiveEffects");
    if (tagActiveEffects)
    {
        //TODO: load potion effect
    }

    nbt::TagList* tagDropChances = tagNbtData->getValueAt<nbt::TagList>("DropChances");
    if (tagDropChances)
    {
        i_slot equipementId = 0;
        const std::vector<nbt::Tag *>& dropChanceList = tagDropChances->getValue();
        for (nbt::Tag* tag : dropChanceList)
        {
            nbt::TagFloat* tagChance = dynamic_cast<nbt::TagFloat*>(tag);
            if (!tagChance)
                continue;

            float chance = tagChance->getValue();
            equipementInventory->SetDropChance(equipementId++, chance);
        }
    }

    return loadSucess;
}

bool LivingEntity::Save(nbt::TagCompound* tagNbtData)
{
    return parent_type::Save(tagNbtData);
}

short LivingEntity::GetMaxHealth()
{
    return DEFAULT_MAX_HEALTH;
}

short LivingEntity::GetHealth()
{
	return health;
}

void LivingEntity::SetCustomName(const std::wstring& customName)
{
    metadataManager.SetEntityMetadata(5, customName);
}

void LivingEntity::MoveLiving(double dx, double dz)
{
    double slowFactor = 0.91;

    if (onGround)
    {
        slowFactor = 0.546;

        i_block blockBottomId = 0;
        if (1 < y && y < 256)
        {
            blockBottomId = world->GetBlockId(floor(x), floor(y) - 1, floor(z));
            if (blockBottomId > 0)
            {
                const Block::Block* blockBottom = Block::BlockList::getBlock(blockBottomId);
                if (blockBottom)
                {
                    slowFactor = blockBottom->GetSlipperiness() * 0.91;
                }
            }
        }
    }
    else
    {
        dx *= 0.02;
        dz *= 0.02;
    }
    motionY *= 0.9800000190734863;
    if (motionY < 0 && !onGround)
        motionY *= fallingSpeedFactor;

    motionX += (1. - slowFactor) * dx;
    motionZ += (1. - slowFactor) * dz;
    motionX *= slowFactor;
    motionZ *= slowFactor;

    Move(motionX, motionY, motionZ);
    motionY -= 0.08;

}

void LivingEntity::SetCustomNameVisible(bool visible)
{
    metadataManager.SetEntityMetadata(6, visible ? char(1) : char(1));
}

void LivingEntity::UpdateTick()
{
    if (dead)
        return;

    if (hurtTime > 0)
        hurtTime--;

    if (livingSoundTimer <= -(rand() % 1000))
    {
        livingSoundTimer = livingSoundInterval;
        PlayLivingSound();
    }
    else
        livingSoundTimer--;
}

void LivingEntity::Attack(LivingEntity* attacker, int& damage)
{
    if (hurtTime > 0)
    {
        damage = -1;
        return;
    }
    hurtTime = MAX_HURTTIME;

    double dx = attacker->x - x;
    double dz = attacker->z - z;

    while (dx * dx + dz * dz < 1.0E-4)
    {
        dz = Util::randFloat(-0.01f, 0.01f);
        dx = Util::randFloat(-0.01f, 0.01f);
    }
    KnockBack(dx, dz);
}

void LivingEntity::DealDamage(int damage)
{
    health -= damage;
    hasTakeDamage = true;
    if (health <= 0)
    {
        Kill();
    }
    PlayHurtSound();
}

void LivingEntity::SetEntityEat()
{
	entityEat = true;
}

void LivingEntity::Kill()
{
    parent_type::Kill();
    PlayDeathSound();
    //TODO: timer
}

void LivingEntity::GetSpecificUpdatePacket(Network::NetworkPacket& packet)
{
    if (hasTakeDamage)
    {
        hasTakeDamage = false;
        packet << (unsigned char)Network::OP_ENTITY_STATUS << entityId << char(2);
    }
    if (entityEat)
    {
    	entityEat = false;
    	packet << (unsigned char)Network::OP_ENTITY_STATUS << entityId << char(10);
    }
}

float LivingEntity::GetSoundVolume() const
{
    return 1.f;
}

float LivingEntity::GetSoundModifier() const
{
    return Util::randFloat(0.8f, 1.2f);
}

void LivingEntity::PlaySound(const std::wstring& sound)
{
    if (world != nullptr)
    {
        float soundModifier = GetSoundModifier();
        float soundVolume = GetSoundVolume();
        world->PlaySound(x, y, z, sound, soundVolume, soundModifier, 2);
    }
}

void LivingEntity::PlayLivingSound()
{
    if (livingSound != L"")
        PlaySound(livingSound);
}

void LivingEntity::PlayHurtSound()
{
    if (hurtSound != L"")
        PlaySound(hurtSound);
}

void LivingEntity::SetDeathSound(const std::wstring& deathSound)
{
    // TODO: use design patern unique to reduce memory usage
    this->deathSound = deathSound;
}

void LivingEntity::SetHurtSound(const std::wstring& hurtSound)
{
    // TODO: use design patern unique to reduce memory usage
    this->hurtSound = hurtSound;
}

void LivingEntity::KnockBack(double dx, double dz)
{
    double distance = std::sqrt(dx * dx + dz * dz);
    double speed = 0.4;
    motionX /= 2.0;
    motionY /= 2.0;
    motionZ /= 2.0;

    // Normalize vector
    motionX -= dx / distance * speed;
    motionY += speed;
    motionZ -= dz / distance * speed;

    if (motionY > 0.4000000059604645)
    {
        motionY = 0.4000000059604645;
    }
}

void LivingEntity::SetLivingSoundInterval(int livingSoundInterval)
{
    this->livingSoundInterval = livingSoundInterval;
}

void LivingEntity::SetLivingSound(const std::wstring& livingSound)
{
    // TODO: use design patern unique to reduce memory usage
    this->livingSound = livingSound;
}

void LivingEntity::PlayDeathSound()
{
    if (deathSound != L"")
        PlaySound(deathSound);
}

} /* namespace World */
