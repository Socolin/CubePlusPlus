#include "Inventory.h"

#include "Entity/EntityPlayer.h"
#include "Network/NetworkPacket.h"
#include "Network/OpcodeList.h"

namespace Inventory
{

Inventory::Inventory(int maxSlot)
    : updatedSlot(4), slot(maxSlot), maxSlot(maxSlot)
{
}

Inventory::~Inventory()
{
}

void Inventory::OpenInventory(World::EntityPlayer* entityPlayer, int offset, i_windowId windowId)
{
    playerWithOffsetList[entityPlayer] = {offset, windowId};
}

void Inventory::CloseInventory(World::EntityPlayer* entityPlayer)
{
    playerWithOffsetList.erase(entityPlayer);
}

ItemStack& Inventory::GetSlot(int slotId)
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

void Inventory::SetSlot(int slotId, const ItemStack itemStack)
{
    slot[slotId] = itemStack;
    updatedSlot.push_back(slotId);
    SendUpdateToAllViewer();// Move it or remove updatedSlot system
}

void Inventory::SendInventoryTo(World::EntityPlayer* entityPlayer, Network::NetworkPacket& packet)
{
    auto playerData = playerWithOffsetList[entityPlayer];
    int offset = playerData.offsetSlot;
    i_windowId windowId = playerData.windowId;
    for (const ItemStack& item : slot)
    {
        packet << item;
    }
}

int Inventory::GetMaxSlot() const
{
    return maxSlot;
}

} /* namespace Inventory */
