#ifndef ENTITYMETADATAMANAGER_H_
#define ENTITYMETADATAMANAGER_H_

#include <map>
#include <string>

#include "EntityMetadata.h"
#include "Inventory/ItemStack.h"

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
    void SetEntityMetadata(int valueId, short value);
    void SetEntityMetadata(int valueId, int value);
    void SetEntityMetadata(int valueId, float value);
    void SetEntityMetadata(int valueId, const std::wstring& value);
    void SetEntityMetadata(int valueId, const Inventory::ItemStack* value);

    char GetCharEntityMetadata(int valueId);
    short GetShortEntityMetadata(int valueId);
    int GetIntEntityMetadata(int valueId);
    float GetFloatEntityMetadata(int valueId);
    const std::wstring& GetStringMetadata(int valueId);
    const Inventory::ItemStack* GetItemEntityMetadata(int valueId);

    bool HasChanged();
    void ClearChange();
private:
    std::map<int, EntityMetadata*> metadataList;
    bool hasChanged; // TODO: maybe change it and use list of change
    std::wstring nullStr;
};

} /* namespace World */
#endif /* ENTITYMETADATAMANAGER_H_ */
