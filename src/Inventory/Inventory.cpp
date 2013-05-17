#include "Inventory.h"

#include "Entity/EntityPlayer.h"
#include "Network/NetworkPacket.h"
#include "Network/OpcodeList.h"
#include "World/World.h"

namespace Inventory
{

Inventory::Inventory(int maxSlot, eInventoryType inventoryType)
    : slot(maxSlot, nullptr), maxSlot(maxSlot), inventoryType(inventoryType)
{
}

Inventory::~Inventory()
{
    for (size_t i = 0; i < slot.size(); i++)
    {
        delete slot[i];
    }
    slot.clear();
}

void Inventory::OpenInventory(World::EntityPlayer* entityPlayer, i_windowId windowId, int offset)
{
    playerWithOffsetList[entityPlayer] = {offset, windowId};
}

void Inventory::CloseInventory(World::EntityPlayer* entityPlayer)
{
    playerWithOffsetList.erase(entityPlayer);
}

void Inventory::CloseInventoryForDelete()
{
    for (auto playerItr : playerWithOffsetList)
    {
        if (playerItr.second.windowId != 0)
            playerItr.first->CloseWindow(playerItr.second.windowId, true);
    }
}

const ItemStack* Inventory::LookSlot(int slotId) const
{
    return slot[slotId];
}

void Inventory::SendUpdateToAllViewer()
{
    if (!updatedSlot.empty())
    {
        Network::NetworkPacket updatePacket;
        for (auto playerItr : playerWithOffsetList)
        {
            int offset = playerItr.second.offsetSlot;
            i_windowId windowId = playerItr.second.windowId;
            for (i_slot slotId : updatedSlot)
            {
                updatePacket << (unsigned char)Network::OP_SET_SLOT << windowId << short(slotId + offset) << slot[slotId];
            }
            playerItr.first->Send(updatePacket);
            updatePacket.Clear();
        }
        updatedSlot.clear();
    }
}

void Inventory::SendInventoryTo(Network::NetworkPacket& packet)
{
    for (const ItemStack* item : slot)
    {
        packet << item;
    }
}

ItemStack* Inventory::TakeSlot(int slotId)
{
    ItemStack* oldItem = slot[slotId];
    slot[slotId] = nullptr;
    updatedSlot.insert(slotId);

    return oldItem;
}
ItemStack* Inventory::TakeAndSetSlot(int slotId, ItemStack* itemStack)
{
    ItemStack* oldItem = slot[slotId];
    slot[slotId] = itemStack;
    updatedSlot.insert(slotId);

    return oldItem;
}

void Inventory::ClearAndSetSlot(int slotId, ItemStack* itemStack)
{
    ItemStack* oldItem = slot[slotId];
    delete oldItem;
    slot[slotId] = itemStack;
    updatedSlot.insert(slotId);

}

void Inventory::ClearSlot(int slotId)
{
    ItemStack* oldItem = slot[slotId];
    delete oldItem;
    slot[slotId] = nullptr;
    updatedSlot.insert(slotId);

}

ItemStack* Inventory::TakeSomeItemInSlot(int slotId, int count)
{
    ItemStack* oldItem = slot[slotId];
    if (oldItem == nullptr)
    {
        return nullptr;
    }
    assert(oldItem->getStackSize() >= count);

    ItemStack* newStack = nullptr;
    if (oldItem->getStackSize() <= count)
    {
        slot[slotId] = nullptr;
        newStack = oldItem;
    }
    else
    {
        newStack = oldItem->Copy();
        newStack->setStackSize(count);
        oldItem->setStackSize(oldItem->getStackSize() - count);
    }
    updatedSlot.insert(slotId);

    return newStack;
}

ItemStack* Inventory::Merge(int slotId, ItemStack* itemStack, int count)
{
    ItemStack* oldItem = slot[slotId];
    if (itemStack == nullptr)
    {
        return nullptr;
    }
    if (count < 0)
    {
        count = itemStack->getStackSize();
    }
    else
    {
        if (count > itemStack->getStackSize())
            count = itemStack->getStackSize();
    }
    ItemStack* returnItem = itemStack;
    if (oldItem == nullptr)
    {
        oldItem = itemStack->Copy();
        oldItem->setStackSize(count);
        itemStack->setStackSize(itemStack->getStackSize() - count);
        slot[slotId] = oldItem;
    }
    else
    {
        if (!oldItem->IsSoftEqual(itemStack))
        {
            return itemStack;
        }

        if (oldItem->GetMaxStackSize() >= (count + oldItem->getStackSize()))
        {
            oldItem->setStackSize(oldItem->getStackSize() + count);
            itemStack->setStackSize(itemStack->getStackSize() - count);
        }
        else
        {
            int freeSpace = oldItem->GetMaxStackSize() - oldItem->getStackSize();
            oldItem->setStackSize(oldItem->getStackSize() + freeSpace);
            itemStack->setStackSize(itemStack->getStackSize() - freeSpace);
        }
    }
    if (itemStack->getStackSize() == 0)
    {
        delete itemStack;
        returnItem = nullptr;
    }
    updatedSlot.insert(slotId);

    return returnItem;
}


int Inventory::GetMaxSlot() const
{
    return maxSlot;
}

void Inventory::DropInventory(World::World* world, double x, double y, double z)
{
    for (ItemStack* item : slot)
    {
        if (item != nullptr)
        {
            world->DropItemstackWithRandomDirection(x, y, z, item);
        }
    }
    for (size_t i = 0; i < slot.size(); i++)
    {
        slot[i] = nullptr;
    }
}

void Inventory::TakeStackableItemAndFillStack(ItemStack* itemStack)
{
    int avaibleSpace = itemStack->GetMaxStackSize() - itemStack->getStackSize();
    for (size_t slotId = 0; slotId < slot.size() && avaibleSpace > 0; slotId++)
    {
        if (avaibleSpace == 0)
            return;
        ItemStack* currentItemStack = slot[slotId];
        if (currentItemStack != nullptr && currentItemStack->IsSoftEqual(itemStack))
        {
            int currentStackSize = currentItemStack->getStackSize();
            if (currentStackSize < currentItemStack->GetMaxStackSize())
            {
                int toTakeCount = std::min(avaibleSpace, currentStackSize);

                itemStack->setStackSize(itemStack->getStackSize() + toTakeCount);
                if (toTakeCount < currentStackSize)
                    currentItemStack->setStackSize(currentItemStack->getStackSize() - toTakeCount);
                else
                {
                    delete currentItemStack;
                    slot[slotId] = nullptr;
                }
                avaibleSpace -= toTakeCount;
                updatedSlot.insert(slotId);
            }
        }
    }
}

ItemStack* Inventory::StackStackableItemFromStack(ItemStack* itemStack)
{
    if (itemStack != nullptr)
    {
        for (size_t slotId = 0; slotId < slot.size() && itemStack != nullptr && itemStack->getStackSize() > 0; slotId++)
        {
            itemStack = Merge(slotId, itemStack);
        }
    }
    return itemStack;
}

eInventoryType Inventory::GetInventoryType()
{
    return inventoryType;
}

} /* namespace Inventory */
