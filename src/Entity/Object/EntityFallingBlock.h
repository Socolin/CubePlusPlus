#ifndef ENTITYFALLINGBLOCK_H_
#define ENTITYFALLINGBLOCK_H_

#include "Entity/Entity.h"
#include "Util/types.h"

namespace World
{

class EntityFallingBlock : public Entity
{
public:
    EntityFallingBlock(double x, double y, double z, i_block blockId, i_data blockData);
    virtual ~EntityFallingBlock();

    virtual void UpdateTick() override;
    virtual void GetSpecificUpdatePacket(Network::NetworkPacket& packet) override;
    virtual void GetCreatePacket(Network::NetworkPacket& packet) override;
private:
    i_block blockId;
    i_data blockData;
};

} /* namespace Scripting */
#endif /* ENTITYFALLINGBLOCK_H_ */
