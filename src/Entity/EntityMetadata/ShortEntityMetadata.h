#ifndef SHORTENTITYMETADATA_H_
#define SHORTENTITYMETADATA_H_

#include "EntityMetadata.h"

namespace World
{

class ShortEntityMetadata: public World::EntityMetadata
{
public:
    ShortEntityMetadata(int valueId, short value);
    virtual ~ShortEntityMetadata();
    virtual void Write(Network::NetworkPacket& packet) override;
private:
    short value;
};

} /* namespace World */
#endif /* SHORTENTITYMETADATA_H_ */
