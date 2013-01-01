#include "LivingEntity.h"

namespace World
{

LivingEntity::LivingEntity(double x, double y, double z) :
        Entity(x, y, z), hasChangeItemInHand(false)
{

}

LivingEntity::~LivingEntity()
{
    // TODO Auto-generated destructor stub
}

} /* namespace World */
