#include "EntityMetadataManager.h"

#include "ItemStackEntityMetadata.h"
#include "CharEntityMetadata.h"
#include "FloatEntityMetadata.h"
#include "IntegerEntityMetadata.h"
#include "ShortEntityMetadata.h"

#include "Network/NetworkPacket.h"

namespace World
{

EntityMetadataManager::EntityMetadataManager()
{
}

EntityMetadataManager::~EntityMetadataManager()
{
}

void EntityMetadataManager::Write(Network::NetworkPacket& packet)
{
    for (auto metadata : metadataList)
    {
        metadata.second->Write(packet);
    }
    packet << (char)127;
}

void EntityMetadataManager::SetEntityMetadata(int valueId, char value)
{
    EntityMetadata* oldMetadata = metadataList[valueId];
    metadataList[valueId] = new CharEntityMetadata(valueId, value);
    if (oldMetadata)
        delete oldMetadata;
}

void EntityMetadataManager::SetEntityMetadata(int valueId, short value)
{
    EntityMetadata* oldMetadata = metadataList[valueId];
    metadataList[valueId] = new ShortEntityMetadata(valueId, value);
    if (oldMetadata)
        delete oldMetadata;
}

void EntityMetadataManager::SetEntityMetadata(int valueId, int value)
{
    EntityMetadata* oldMetadata = metadataList[valueId];
    metadataList[valueId] = new IntegerEntityMetadata(valueId, value);
    if (oldMetadata)
        delete oldMetadata;
}

void EntityMetadataManager::SetEntityMetadata(int valueId, float value)
{
    EntityMetadata* oldMetadata = metadataList[valueId];
    metadataList[valueId] = new FloatEntityMetadata(valueId, value);
    if (oldMetadata)
        delete oldMetadata;
}

void EntityMetadataManager::SetEntityMetadata(int valueId, const Inventory::ItemStack* value)
{
    EntityMetadata* oldMetadata = metadataList[valueId];
    metadataList[valueId] = new ItemStackEntityMetadata(valueId, value);
    if (oldMetadata)
        delete oldMetadata;
}

char EntityMetadataManager::GetCharEntityMetadata(int valueId)
{
    EntityMetadata* metadata = metadataList[valueId];
    if (metadata != nullptr)
    {
        if (metadata->getType() == EntityMetadata::DATATYPE_CHAR)
        {
            CharEntityMetadata* charMetadata = dynamic_cast<CharEntityMetadata*>(metadata);
            return charMetadata->getValue();
        }
    }
    return 0;
}

short EntityMetadataManager::GetShortEntityMetadata(int valueId)
{
    EntityMetadata* metadata = metadataList[valueId];
    if (metadata != nullptr)
    {
        if (metadata->getType() == EntityMetadata::DATATYPE_SHORT)
        {
            ShortEntityMetadata* shortMetadata = dynamic_cast<ShortEntityMetadata*>(metadata);
            return shortMetadata->getValue();
        }
    }
    return 0;
}

int EntityMetadataManager::GetIntEntityMetadata(int valueId)
{
    EntityMetadata* metadata = metadataList[valueId];
    if (metadata != nullptr)
    {
        if (metadata->getType() == EntityMetadata::DATATYPE_INTEGER)
        {
            IntegerEntityMetadata* intMetadata = dynamic_cast<IntegerEntityMetadata*>(metadata);
            return intMetadata->getValue();
        }
    }
    return 0;
}

float EntityMetadataManager::GetFloatEntityMetadata(int valueId)
{
    EntityMetadata* metadata = metadataList[valueId];
    if (metadata != nullptr)
    {
        if (metadata->getType() == EntityMetadata::DATATYPE_FLOAT)
        {
            FloatEntityMetadata* floatMetadata = dynamic_cast<FloatEntityMetadata*>(metadata);
            return floatMetadata->getValue();
        }
    }
    return 0;
}

const Inventory::ItemStack* EntityMetadataManager::GetItemEntityMetadata(int valueId)
{
    EntityMetadata* metadata = metadataList[valueId];
    if (metadata != nullptr)
    {
        if (metadata->getType() == EntityMetadata::DATATYPE_ITEMSTACK)
        {
            ItemStackEntityMetadata* itemMetadata = dynamic_cast<ItemStackEntityMetadata*>(metadata);
            return itemMetadata->getValue();
        }
    }
    return nullptr;
}

} /* namespace World */
