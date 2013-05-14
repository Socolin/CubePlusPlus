#include "Inventory.h"

#include "Entity/EntityPlayer.h"
#include "Network/NetworkPacket.h"
#include "Network/OpcodeList.h"

namespace Inventory
{

Inventory::Inventory(int maxSlot)
    : slot(maxSlot, nullptr), maxSlot(maxSlot)
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

void Inventory::SendInventoryTo(World::EntityPlayer* entityPlayer, Network::NetworkPacket& packet)
{
    auto playerData = playerWithOffsetList[entityPlayer];
    int offset = playerData.offsetSlot;
    i_windowId windowId = playerData.windowId;
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

} /* namespace Inventory */
