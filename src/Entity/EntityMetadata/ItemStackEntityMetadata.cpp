#include "ItemStackEntityMetadata.h"

#include "Network/NetworkPacket.h"
namespace World
{

ItemStackEntityMetadata::ItemStackEntityMetadata(int valueId,const Inventory::ItemStack* value)
    : EntityMetadata(valueId, DATATYPE_ITEMSTACK)
    , value(value)
{
}

ItemStackEntityMetadata::~ItemStackEntityMetadata()
{
    delete value;
}

void ItemStackEntityMetadata::Write(Network::NetworkPacket& packet)
{
    char header = (DATATYPE_ITEMSTACK<< 5) | valueId;
    packet << header << value;
}

const Inventory::ItemStack* ItemStackEntityMetadata::getValue() const
{
    return value;
}

} /* namespace World */
