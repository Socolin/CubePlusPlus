#include "FloatEntityMetadata.h"

#include "Network/NetworkPacket.h"

namespace World
{

FloatEntityMetadata::FloatEntityMetadata(int valueId, float value)
    : EntityMetadata(valueId, DATATYPE_FLOAT)
    , value(value)
{
}

FloatEntityMetadata::~FloatEntityMetadata()
{
}

void FloatEntityMetadata::Write(Network::NetworkPacket& packet)
{
    char header = (DATATYPE_FLOAT << 5) | valueId;
    packet << header << value;
}

float FloatEntityMetadata::getValue() const
{
    return value;
}

} /* namespace World */
