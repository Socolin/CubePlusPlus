#ifndef ENTITYITEM_H_
#define ENTITYITEM_H_

#include "Entity/Entity.h"

#include "Inventory/Inventory.h"

namespace Inventory
{
class ItemStack;
}
namespace World
{

class EntityItem : public Entity
{
public:
    EntityItem(double x, double y, double z, Inventory::ItemStack* itemStack, double motionX, double motionY, double motionZ, int timeBeforePickup = 10);
    virtual ~EntityItem();

    virtual void UpdateTick() override;

    virtual void GetSpecificUpdatePacket(Network::NetworkPacket& packet);
    virtual void GetCreatePacket(Network::NetworkPacket& packet);

    virtual void OnCollideWithPlayer(EntityPlayer* player);

    const Inventory::ItemStack* LookStoreItem() const;
private:
    unsigned int liveTime;
    int timeBeforePickup;
    Inventory::Inventory storedItem;
};

} /* namespace World */
#endif /* ENTITYITEM_H_ */
