#include "CharEntityMetadata.h"

#include "Network/NetworkPacket.h"

namespace World
{

CharEntityMetadata::CharEntityMetadata(int valueId, char value)
    : EntityMetadata(valueId, DATATYPE_CHAR)
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

char CharEntityMetadata::getValue() const
{
    return value;
}

} /* namespace World */
