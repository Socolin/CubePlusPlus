#include "ShortEntityMetadata.h"

#include "Network/NetworkPacket.h"

namespace World
{

ShortEntityMetadata::ShortEntityMetadata(int valueId, short value)
    : EntityMetadata(valueId)
    , value(value)
{
}

ShortEntityMetadata::~ShortEntityMetadata()
{
}

void ShortEntityMetadata::Write(Network::NetworkPacket& packet)
{
    char header = (DATATYPE_SHORT << 5) | valueId;
    packet << header << value;
}

} /* namespace World */
