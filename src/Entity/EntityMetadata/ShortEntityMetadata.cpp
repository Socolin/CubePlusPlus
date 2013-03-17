#include "ShortEntityMetadata.h"

#include "Network/NetworkPacket.h"

namespace World
{

ShortEntityMetadata::ShortEntityMetadata(int valueId, short value)
    : EntityMetadata(valueId, DATATYPE_SHORT)
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

const short ShortEntityMetadata::getValue() const
{
    return value;
}

} /* namespace World */
