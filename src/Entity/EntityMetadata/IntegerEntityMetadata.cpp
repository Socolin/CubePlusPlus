#include "IntegerEntityMetadata.h"

#include "Network/NetworkPacket.h"

namespace World
{

IntegerEntityMetadata::IntegerEntityMetadata(int valueId, int value)
    : EntityMetadata(valueId)
    , value(value)
{
}

IntegerEntityMetadata::~IntegerEntityMetadata()
{
}

void IntegerEntityMetadata::Write(Network::NetworkPacket& packet)
{
    char header = (DATATYPE_INTEGER << 5) | valueId;
    packet << header << value;
}

} /* namespace World */
