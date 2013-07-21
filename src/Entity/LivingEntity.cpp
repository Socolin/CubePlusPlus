#include "LivingEntity.h"

#include <cppnbt.h>

#include "Util/StringUtil.h"
#include "Inventory/InventoryEntityEquipement.h"
#include "Network/NetworkPacket.h"
#include "Network/OpcodeList.h"

namespace World
{

LivingEntity::LivingEntity(eEntityType entityType, int entityTypeFlag, double x, double y, double z) :
    Entity(entityType, entityTypeFlag | ENTITY_TYPEFLAG_LIVING_ENTITY, x, y, z)
    , hasChangeItemInHand(false)
    , hasTakeDamage(false)
    , health(DEFAULT_MAX_HEALTH)
    , hurtTime(0)
    , deathTime(0)
    , attackTime(0)
    , canPickUpLoot(false)
    , persistenceRequired(false)
    , equipementInventory(nullptr)
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

void LivingEntity::SetCustomName(const std::wstring& customName)
{
    metadataManager.SetEntityMetadata(5, customName);
}

void LivingEntity::MoveLiving(double dx, double dz)
{
    motionY -= 0.03999999910593033;
    motionY *= 0.9800000190734863;
    if (motionY < 0 && !onGround)
        motionY *= fallingSpeedFactor;
    Move(dx, motionY, dz);
}

void LivingEntity::SetCustomNameVisible(bool visible)
{
    metadataManager.SetEntityMetadata(6, visible ? char(1) : char(1));
}

void LivingEntity::UpdateTick()
{
    if (hurtTime > 0)
        hurtTime--;
}

void LivingEntity::Attack(LivingEntity* /*attacker*/, int& damage)
{
    if (hurtTime > 0)
    {
        damage = -1;
        return;
    }
    hurtTime = MAX_HURTTIME;
}

void LivingEntity::DealDamage(int damage)
{
    health -= damage;
    hasTakeDamage = true;
    if (health <= 0)
    {
        Kill();
    }
}

void LivingEntity::GetSpecificUpdatePacket(Network::NetworkPacket& packet)
{
    if (hasTakeDamage)
    {
        hasTakeDamage = false;
        packet << (unsigned char) Network::OP_ENTITY_STATUS << entityId << char(2);
    }
}

} /* namespace World */
