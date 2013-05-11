#ifndef INVENTORY_H_
#define INVENTORY_H_

#include <map>
#include <vector>

#include "ItemStack.h"
#include "Util/types.h"

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

    void OpenInventory(World::EntityPlayer* entityPlayer, int offset);
    void CloseInventory(World::EntityPlayer* entityPlayer);
    void SendUpdateToAllViewer();

    virtual ItemStack& GetSlot(int slotId);
    virtual void SetSlot(int slotId, const ItemStack& itemStack);

    virtual void SendInventoryTo(World::EntityPlayer* entityPlayer) = 0;
protected:
    std::map<World::EntityPlayer*, int /*offset slot*/> playerWithOffsetList;
    std::vector<i_slot> updatedSlot; // TODO: see to use set instead vector
    std::vector<ItemStack> slot;
    int maxSlot;
};

} /* namespace Inventory */
#endif /* INVENTORY_H_ */
