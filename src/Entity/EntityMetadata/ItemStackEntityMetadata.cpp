#include "ItemStackEntityMetadata.h"

#include "Network/NetworkPacket.h"
namespace World
{

ItemStackEntityMetadata::ItemStackEntityMetadata(int valueId, Inventory::ItemStack& value)
    : EntityMetadata(valueId)
    , value(value)
{
}

ItemStackEntityMetadata::~ItemStackEntityMetadata()
{
}

void ItemStackEntityMetadata::Write(Network::NetworkPacket& packet)
{
    char header = (DATATYPE_CHAR << 5) | valueId;
    packet << header << value;
}

} /* namespace World */
