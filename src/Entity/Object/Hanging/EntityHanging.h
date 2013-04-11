#ifndef ENTITYHANGING_H_
#define ENTITYHANGING_H_

#include "Entity/Entity.h"
#include "Util/types.h"

namespace World
{

class EntityHanging : public Entity
{
public:
    EntityHanging(eEntityType type, int x, i_height y, int z, int direction);
    virtual ~EntityHanging();

    virtual void UpdateTick() override;
    void setDirection(int direction);
protected:
    virtual int getPixelWidth() const = 0;
    virtual int getPixelHeight() const = 0;
    bool isOnValidSurface();
private:
    double getWidthOffset(int width);
protected:
    int direction;
    int blockX;
    i_height blockY;
    int blockZ;
    int updateTimer;
};

} /* namespace World */
#endif /* ENTITYHANGING_H_ */
