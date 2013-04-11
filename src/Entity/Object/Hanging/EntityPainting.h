#ifndef ENTITYPAINTING_H_
#define ENTITYPAINTING_H_

#include "EntityHanging.h"

#include "Database/MiscData/PaintingData.h"
#include "Inventory/ItemStack.h"

namespace World
{

class World;
class EntityPainting : public EntityHanging
{
public:
    EntityPainting(int x, i_height y, int z, int direction, Inventory::ItemStack& item, World* world);
    virtual ~EntityPainting();

    void SelectRandomPainting(Inventory::ItemStack& itemStack);
    void GetCreatePacket(Network::NetworkPacket& packet);
    void GetSpecificUpdatePacket(Network::NetworkPacket& packet);
protected:
    virtual int getPixelWidth() const;
    virtual int getPixelHeight() const;
private:
    Database::PaintingData::Painting art;
};

} /* namespace World */

#endif /* ENTITYPAINTING_H_ */
