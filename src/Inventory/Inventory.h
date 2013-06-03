#ifndef INVENTORY_H_
#define INVENTORY_H_

#include <map>
#include <set>
#include <vector>

#include "InventoryConstants.h"
#include "ItemStack.h"
#include "Util/types.h"

namespace nbt
{
class TagList;
}
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
     * Take 'count' item from slot, if count > stackSize, then get all stack
     * else do a copy of slot with stackSize=count
     * @param slotId id of slot where is item to take
     * @param count number of item to take
     * @return item taken
     */
    ItemStack* TakeSomeItemInSlot(int slotId, int count);

    /**
     * Decrease stackSize of slot, if stackSize <= 0 then the item is deleted
     * @param slotId id of slot where is item
     * @param count number of item to remove
     */
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
    virtual void DropInventory(World::World* world, double x, double y, double z);

    virtual void DropInventory(World::EntityPlayer* player);

    /**
     * Walk through the inventory and take all stackable item to stack them into 'itemStack' while it's not full
     * @param itemStack item stack to fill
     * @param takeFullStack
     */
    void TakeStackableItemAndFillStack(ItemStack* itemStack, bool takeFullStack);

    /**
     * Walk through the inventory and try to place in free slot and in other stackable item the items contained in
     * 'itemStack'
     * @param itemStack items which must be place in slot
     * @param reverseOrder start from end of inventory
     * @param fillEmptySlot if false, it will ignore empty slot while walink throuh inventory
     * @return The remaining items
     */
    ItemStack* StackStackableItemFromStack(ItemStack* itemStack, bool reverseOrder, bool fillEmptySlot);

    /**
     * Return inventory type, used for shift click
     * @return
     */
    eInventoryType GetInventoryType();

    /**
     * Test if a slot can be fill by a player or not, like for result slot of crafting table or furnace
     * the player can only take item from this slot, and cannot place it.
     * @param slotId the id of slot
     * @return true if player can fill the slot
     */
    virtual bool CanPlayerPlaceItemAt(i_slot slotId);

    /**
     * Send window property update to all viewer, ex: furnace progress bar
     * @param property propertyId
     * @param value new value
     */
    virtual void UpdateWindowProperty(short property, short value);

    /**
     * Count the number of item that can be place in inventory
     * @param item item to place
     * @return number of item
     */
    int CountAvaibleSpaceForItem(const ItemStack* item);

    /**
     * Get number of player whose has opened the inventory
     * @return number of player with this inventory openned
     */
    int GetPlayerCount() const;

    /**
     * Load inventory from data
     * @param nbtData
     */
    virtual void Load(nbt::TagList* nbtData);
protected:
    struct playerData
    {
        int offsetSlot;
        i_windowId windowId;
    };
    std::map<World::EntityPlayer*, playerData> playerWithOffsetList;
    std::vector<i_slot> updatedSlot;
    std::vector<ItemStack*> slot;
    int maxSlot;
    eInventoryType inventoryType;
};

} /* namespace Inventory */
#endif /* INVENTORY_H_ */
