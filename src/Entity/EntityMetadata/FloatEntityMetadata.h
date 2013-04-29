#ifndef FLOATENTITYMETADATA_H_
#define FLOATENTITYMETADATA_H_

#include "EntityMetadata.h"

namespace World
{

class FloatEntityMetadata: public EntityMetadata
{
public:
    FloatEntityMetadata(int valueId, float value);
    virtual ~FloatEntityMetadata();
    virtual void Write(Network::NetworkPacket& packet) override;
    float getValue() const;

private:
    const float value;
};

} /* namespace World */
#endif /* FLOATENTITYMETADATA_H_ */
