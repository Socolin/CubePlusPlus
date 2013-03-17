#ifndef INTEGERENTITYMETADATA_H_
#define INTEGERENTITYMETADATA_H_

#include "EntityMetadata.h"

namespace World
{

class IntegerEntityMetadata: public World::EntityMetadata
{
public:
    IntegerEntityMetadata(int valueId, int value);
    virtual ~IntegerEntityMetadata();
    virtual void Write(Network::NetworkPacket& packet) override;
    const int getValue() const;

private:
    const int value;
};

} /* namespace World */
#endif /* INTEGERENTITYMETADATA_H_ */
