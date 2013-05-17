#ifndef INVENTORY_H_
#define INVENTORY_H_

#include <map>
#include <set>
#include <vector>

#include "InventoryConstants.h"
#include "ItemStack.h"
#include "Util/types.h"

namespace Network
{
class NetworkPacket;
}
namespace World
{
class EntityPlayer;
class World;
}
namespace Inventory
{
class ItemStack;
class Inventory
{
public:
    /**
     * Create a new inventory with maxSlot slot number
     * @param maxSlot number of slot in inventory
     */
    Inventory(int maxSlot, eInventoryType inventoryType = INVENTORY_TYPE_DEFAULT);

    /**
     * Destroy all item in inventory, if inventory must drop item, it must be done before
     */
    virtual ~Inventory();

    /**
     * Open inventory for player, so the player is added to a list and he receive all change done in this inventory
     * @param entityPlayer the player which open inventory
     * @param windowId the window id in which this inventory is contained for player
     * @param offset offset of first slot for player inventory
     */
    void OpenInventory(World::EntityPlayer* entityPlayer, i_windowId windowId, int offset);

    /**
     * Remove player from looker list
     * @param entityPlayer
     */
    void CloseInventory(World::EntityPlayer* entityPlayer);

    /**
     * Close inventory and ask to all player with this inventory open, to close the windows which use it
     */
    void CloseInventoryForDelete();

    /**
     * Send update packet for all slot which has been change from last time
     */
    void SendUpdateToAllViewer();

    /**
     * Get the count of item from the slot
     * @param slotId
     * @param count
     * @return
     */
    ItemStack* TakeSomeItemInSlot(int slotId, int count);

    void RemoveSomeItemInSlot(int slotId, int count);

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

    /**
     * Fill packet with all slot of inventory, same empty slot
     * @param packet packet to fill
     */
    void SendInventoryTo(Network::NetworkPacket& packet);

    /**
     * Get slot count in inventory
     * @return number of slot in inventory
     */
    int GetMaxSlot() const;

    /**
     * Drop all inventory at coordinate x y z in specified world
     * @param world the world
     * @param x coordinate
     * @param y coordinate
     * @param z coordinate
     */
    void DropInventory(World::World* world, double x, double y, double z);

    void TakeStackableItemAndFillStack(ItemStack* itemStack);

    ItemStack* StackStackableItemFromStack(ItemStack* itemStack, bool reverseOrder, bool fillEmptySlot);

    eInventoryType GetInventoryType();

    virtual bool CanPlayerPlaceItemAt(i_slot slotId);

    virtual void UpdateWindowProperty(short property, short value);
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
    eInventoryType inventoryType;
};

} /* namespace Inventory */
#endif /* INVENTORY_H_ */
