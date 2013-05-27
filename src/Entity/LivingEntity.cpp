#include "LivingEntity.h"

namespace World
{

LivingEntity::LivingEntity(eEntityType entityType, int entityTypeFlag, double x, double y, double z) :
    Entity(entityType, entityTypeFlag | ENTITY_TYPEFLAG_LIVING_ENTITY, x, y, z), hasChangeItemInHand(false)
{

}

LivingEntity::~LivingEntity()
{
}

float LivingEntity::getEyeHeight()
{
    return 1.62f;
}

} /* namespace World */
