#include "LivingEntity.h"

namespace World
{

LivingEntity::LivingEntity(eEntityType entityType, double x, double y, double z) :
    Entity(entityType, x, y, z), hasChangeItemInHand(false)
{

}

LivingEntity::~LivingEntity()
{
    // TODO Auto-generated destructor stub
}

float LivingEntity::getEyeHeight()
{
    return 1.62f;
}

} /* namespace World */
