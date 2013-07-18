#ifndef ENTITYHANGINGFRAME_H_
#define ENTITYHANGINGFRAME_H_

#include "EntityHanging.h"

#define ENTITY_HANGING_FRAME_METADATA_ITEM 2
#define ENTITY_HANGING_FRAME_METADATA_ITEM_ROTATION 3

namespace World
{

class EntityHangingFrame : public EntityHanging
{
public:
    EntityHangingFrame(int x, i_height y, int z, int direction);
    virtual ~EntityHangingFrame();
    virtual void Interact(EntityPlayer* player) override;
    virtual void GetCreatePacket(Network::NetworkPacket& packet) override;
    virtual void GetSpecificUpdatePacket(Network::NetworkPacket& packet) override;
protected:
    virtual int getPixelWidth() const override;
    virtual int getPixelHeight() const override;
};

} /* namespace World */
#endif /* ENTITYHANGINGFRAME_H_ */
