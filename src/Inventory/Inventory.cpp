#include "Inventory.h"

namespace Inventory
{

Inventory::Inventory(int maxSlot)
    : updatedSlot(4), slot(maxSlot), maxSlot(maxSlot)
{
}

Inventory::~Inventory()
{
}

void Inventory::OpenInventory(World::EntityPlayer* entityPlayer, int offset)
{
    playerWithOffsetList[entityPlayer] = offset;
    SendInventoryTo(entityPlayer);
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
        for (auto playerItr : playerWithOffsetList)
        {
            for (i_slot slotId : updatedSlot)
            {
                // TODO:
                //playerItr->first;
                //playerItr.second + slotId << value of slotId to send to player;
            }
        }
        updatedSlot.clear();
    }
}

void Inventory::SetSlot(int slotId, const ItemStack& itemStack)
{
    slot[slotId] = itemStack;
    updatedSlot.push_back(slotId);
}

} /* namespace Inventory */
