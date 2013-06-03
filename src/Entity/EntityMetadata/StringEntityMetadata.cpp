#include "StringEntityMetadata.h"

#include "Network/NetworkPacket.h"

namespace World
{

StringEntityMetadata::StringEntityMetadata(int valueId, const std::wstring& value)
    : EntityMetadata(valueId, DATATYPE_STRING)
    , value(value)
{
}

StringEntityMetadata::~StringEntityMetadata()
{
}

void StringEntityMetadata::Write(Network::NetworkPacket& packet)
{
    char header = (DATATYPE_STRING << 5) | valueId;
    packet << header << value;
}

const std::wstring& StringEntityMetadata::getValue() const
{
    return value;
}

} /* namespace World */
