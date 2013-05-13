#ifndef ENTITYITEM_H_
#define ENTITYITEM_H_

#include "Entity/Entity.h"

#include "Inventory/Inventory.h"

namespace World
{

class EntityItem : public Entity
{
public:
    EntityItem(double x, double y, double z, Inventory::ItemStack* itemStack, double motionX, double motionY, double motionZ);
    virtual ~EntityItem();

    virtual void UpdateTick() override;

    virtual void GetSpecificUpdatePacket(Network::NetworkPacket& packet);
    virtual void GetCreatePacket(Network::NetworkPacket& packet);
private:
    unsigned int liveTime;
    Inventory::Inventory storedItem;
};

} /* namespace World */
#endif /* ENTITYITEM_H_ */
