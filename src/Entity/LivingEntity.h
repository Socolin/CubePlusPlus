#ifndef LIVINGENTITY_H_
#define LIVINGENTITY_H_

#include "Entity.h"

namespace World
{

class LivingEntity: public Entity
{
public:
    LivingEntity(double x, double y, double z);
    virtual ~LivingEntity();
    virtual float getEyeHeight();
protected:
    bool hasChangeItemInHand;
};

} /* namespace World */
#endif /* LIVINGENTITY_H_ */
