#include "CharEntityMetadata.h"

#include "Network/NetworkPacket.h"
namespace World
{

CharEntityMetadata::CharEntityMetadata(int valueId, char value)
    : EntityMetadata(valueId)
    , value(value)
{
}

CharEntityMetadata::~CharEntityMetadata()
{
}

void CharEntityMetadata::Write(Network::NetworkPacket& packet)
{
    char header = (DATATYPE_CHAR << 5) | valueId;
    packet << header << value;
}

} /* namespace World */
