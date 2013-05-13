#include "Inventory.h"

#include "Entity/EntityPlayer.h"
#include "Network/NetworkPacket.h"
#include "Network/OpcodeList.h"

namespace Inventory
{

Inventory::Inventory(int maxSlot)
    : updatedSlot(4), slot(maxSlot, nullptr), maxSlot(maxSlot)
{
}

Inventory::~Inventory()
{
    for (int i = 0; i < slot.size(); i++)
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
    updatedSlot.push_back(slotId);
    SendUpdateToAllViewer();// Move it or remove updatedSlot system}
    return oldItem;
}
ItemStack* Inventory::TakeAndSetSlot(int slotId, ItemStack* itemStack)
{
    ItemStack* oldItem = slot[slotId];
    slot[slotId] = itemStack;
    updatedSlot.push_back(slotId);
    SendUpdateToAllViewer();// Move it or remove updatedSlot system}
    return oldItem;
}

void Inventory::ClearAndSetSlot(int slotId, ItemStack* itemStack)
{
    ItemStack* oldItem = slot[slotId];
    delete oldItem;
    slot[slotId] = itemStack;
    updatedSlot.push_back(slotId);
    SendUpdateToAllViewer();// Move it or remove updatedSlot system}
}

void Inventory::ClearSlot(int slotId)
{
    ItemStack* oldItem = slot[slotId];
    delete oldItem;
    slot[slotId] = nullptr;
    updatedSlot.push_back(slotId);
    SendUpdateToAllViewer();// Move it or remove updatedSlot system}
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
    updatedSlot.push_back(slotId);
    SendUpdateToAllViewer();// Move it or remove updatedSlot system}
    return newStack;
}

int Inventory::GetMaxSlot() const
{
    return maxSlot;
}

} /* namespace Inventory */
