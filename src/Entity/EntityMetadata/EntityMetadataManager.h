#ifndef ENTITYMETADATAMANAGER_H_
#define ENTITYMETADATAMANAGER_H_

#include "EntityMetadata.h"
#include <map>

namespace Inventory
{
class ItemStack;
}
namespace Network
{
class NetworkPacket;
}
namespace World
{

class EntityMetadataManager
{
public:
    EntityMetadataManager();
    virtual ~EntityMetadataManager();
    void Write(Network::NetworkPacket& packet);
    void SetEntityMetadata(int valueId, char value);
    void SetEntityMetadata(int valueId, Inventory::ItemStack& value);
private:
    std::map<int, EntityMetadata*> metadataList;
};

} /* namespace World */
#endif /* ENTITYMETADATAMANAGER_H_ */
