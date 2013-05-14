#ifndef INVENTORY_H_
#define INVENTORY_H_

#include <map>
#include <set>
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

    void OpenInventory(World::EntityPlayer* entityPlayer, i_windowId windowId, int offset);
    void CloseInventory(World::EntityPlayer* entityPlayer);
    void SendUpdateToAllViewer();


    /**
     * Get the count of item from the slot
     * @param slotId
     * @param count
     * @return
     */
    ItemStack* TakeSomeItemInSlot(int slotId, int count);

    /**
     * Return the item in slot, it must only be use to "look"
     * it must not be use to insert in another slot or store it in other place
     * @param slotId
     * @return
     */
    const ItemStack* LookSlot(int slotId) const;

    /**
     * Empty the slot, it must be done when an item is removed to be used after
     * if the item is no more used after, it must be delete
     * @param slotId the id of the slot
     * @return The item which was in slot
     */
    ItemStack* TakeSlot(int slotId);

    /**
     * Empty the slot and place an other item
     * if the item is no more used after, it must be delete
     * @param slotId the id of the slot
     * @param itemStack The item to place in slot, no other reference must be keep on it
     * @return The item which was in slot
     */
    ItemStack* TakeAndSetSlot(int slotId, ItemStack* itemStack);

    /**
     * Delete item in slot and set new item at the place
     * @param slotId the id of the slot
     * @param itemStack the item to set, no other reference must be keep on it
     */
    void ClearAndSetSlot(int slotId, ItemStack* itemStack);

    /**
     * Delete item
     * @param slotId the id of the slot
     */
    void ClearSlot(int slotId);

    /**
     * Fill stack in inventory to complete the stack, and return the non stackable item
     * @param slotId the id of the slot
     * @param itemStack The item to place in slot, no other reference must be keep on it
     * @param count quantity to item to stack
     * @return The not stacked item
     */
    ItemStack* Merge(int slotId, ItemStack* itemStack, int count = -1);

    void SendInventoryTo(World::EntityPlayer* entityPlayer, Network::NetworkPacket& packet);
    int GetMaxSlot() const;

protected:
    struct playerData
    {
        int offsetSlot;
        i_windowId windowId;
    };
    std::map<World::EntityPlayer*, playerData> playerWithOffsetList;
    std::set<i_slot> updatedSlot;
    std::vector<ItemStack*> slot;
    int maxSlot;
};

} /* namespace Inventory */
#endif /* INVENTORY_H_ */
