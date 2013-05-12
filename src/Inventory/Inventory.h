#ifndef INVENTORY_H_
#define INVENTORY_H_

#include <map>
#include <vector>

#include "ItemStack.h"
#include "Util/types.h"

namespace Network
{
class NetworkPacket;
}
namespace World
{
class EntityPlayer;
}
namespace Inventory
{
class ItemStack;
class Inventory
{
public:
    Inventory(int maxSlot);
    virtual ~Inventory();

    void OpenInventory(World::EntityPlayer* entityPlayer, int offset, i_windowId windowId);
    void CloseInventory(World::EntityPlayer* entityPlayer);
    void SendUpdateToAllViewer();

    virtual ItemStack& GetSlot(int slotId);
    virtual void SetSlot(int slotId, const ItemStack itemStack);

    void SendInventoryTo(World::EntityPlayer* entityPlayer, Network::NetworkPacket& packet);
    int GetMaxSlot() const;

protected:
    struct playerData
    {
        int offsetSlot;
        i_windowId windowId;
    };
    std::map<World::EntityPlayer*, playerData> playerWithOffsetList;
    std::vector<i_slot> updatedSlot; // TODO: see to use set instead vector
    std::vector<ItemStack> slot;
    int maxSlot;
};

} /* namespace Inventory */
#endif /* INVENTORY_H_ */
