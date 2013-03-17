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

void EntityMetadataManager::SetEntityMetadata(int valueId, Inventory::ItemStack& value)
{
    EntityMetadata* oldMetadata = metadataList[valueId];
    metadataList[valueId] = new ItemStackEntityMetadata(valueId, value);
    if (oldMetadata)
        delete oldMetadata;
}

} /* namespace World */
