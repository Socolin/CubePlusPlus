#include "Inventory.h"

#include <NBTField/NBTField.h>

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
    updatedSlot.push_back(slotId);

    return oldItem;
}
ItemStack* Inventory::TakeAndSetSlot(int slotId, ItemStack* itemStack)
{
    ItemStack* oldItem = slot[slotId];
    slot[slotId] = itemStack;
    updatedSlot.push_back(slotId);

    return oldItem;
}

void Inventory::ClearAndSetSlot(int slotId, ItemStack* itemStack)
{
    ItemStack* oldItem = slot[slotId];
    delete oldItem;
    slot[slotId] = itemStack;
    updatedSlot.push_back(slotId);

}

void Inventory::ClearSlot(int slotId)
{
    ItemStack* oldItem = slot[slotId];
    delete oldItem;
    slot[slotId] = nullptr;
    updatedSlot.push_back(slotId);

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

    return newStack;
}


void Inventory::RemoveSomeItemInSlot(int slotId, int count)
{
    ItemStack* oldItem = slot[slotId];
    if (oldItem == nullptr)
    {
        return;
    }
    oldItem->setStackSize(oldItem->getStackSize() - count);
    if (oldItem->getStackSize() <= 0)
    {
        delete oldItem;
        slot[slotId] = nullptr;
    }
    updatedSlot.push_back(slotId);
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
    count = std::min(itemStack->GetMaxStackSize(), count);
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
        if (oldItem->IsStackable(itemStack))
        {
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
    }
    if (itemStack->getStackSize() == 0)
    {
        delete itemStack;
        returnItem = nullptr;
    }
    updatedSlot.push_back(slotId);

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
void Inventory::DropInventory(World::EntityPlayer* player)
{
    for (ItemStack* item : slot)
    {
        if (item != nullptr)
        {
            player->DropItem(item);
        }
    }
    for (size_t i = 0; i < slot.size(); i++)
    {
        slot[i] = nullptr;
    }
}

void Inventory::TakeStackableItemAndFillStack(ItemStack* itemStack, bool takeFullStack)
{
    int avaibleSpace = itemStack->GetMaxStackSize() - itemStack->getStackSize();
    for (size_t slotId = 0; slotId < slot.size() && avaibleSpace > 0; slotId++)
    {
        if (avaibleSpace == 0)
            return;
        ItemStack* currentItemStack = slot[slotId];
        if (currentItemStack != nullptr && currentItemStack->IsStackable(itemStack))
        {
            if (!takeFullStack && currentItemStack->Full())
                continue;

            int currentStackSize = currentItemStack->getStackSize();
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
            updatedSlot.push_back(slotId);
        }
    }
}

ItemStack* Inventory::StackStackableItemFromStack(ItemStack* itemStack, bool reverseOrder, bool fillEmptySlot)
{
    if (itemStack != nullptr)
    {
        size_t start = 0;
        int step = 1;
        if (reverseOrder)
        {
            start = slot.size() - 1;
            step = -1;
        }
        for (size_t slotId = start; slotId < slot.size() && itemStack != nullptr && itemStack->getStackSize() > 0; slotId += step)
        {
            if (fillEmptySlot)
            {
                itemStack = Merge(slotId, itemStack);
            }
            else
            {
                if (slot[slotId] != nullptr)
                    itemStack = Merge(slotId, itemStack);
            }
        }
    }
    return itemStack;
}

eInventoryType Inventory::GetInventoryType()
{
    return inventoryType;
}

bool Inventory::CanPlayerPlaceItemAt(i_slot /*slotId*/)
{
    return true;
}

const ItemStack* Inventory::LookSlot(int slotId) const
{
    return slot[slotId];
}

void Inventory::UpdateWindowProperty(short property, short value)
{
    Network::NetworkPacket updatePacket;
    for (auto playerItr : playerWithOffsetList)
    {
        i_windowId windowId = playerItr.second.windowId;
        updatePacket << (unsigned char)Network::OP_UPDATE_WINDOW_PROPERTY << windowId << property << value;
        playerItr.first->Send(updatePacket);
        updatePacket.Clear();
    }
    updatedSlot.clear();
}

int Inventory::CountAvaibleSpaceForItem(const ItemStack* item)
{
    int count = 0;
    i_stackSize itemStackSize = item->GetMaxStackSize();
    for (ItemStack* currentItem : slot)
    {
        if (currentItem != nullptr)
        {
            if (currentItem->IsStackable(item))
            {
                count += (itemStackSize - currentItem->getStackSize());
            }
        }
        else
        {
            count += itemStackSize;
        }
    }
    return count;
}

int Inventory::GetPlayerCount() const
{
    return playerWithOffsetList.size();
}

void Inventory::Load(NBT::TagList* nbtData)
{
    const std::vector<NBT::Tag *>& itemList = nbtData->GetTagList();
    for (NBT::Tag* tag : itemList)
    {
        NBT::TagCompound* itemData = dynamic_cast<NBT::TagCompound*>(tag);
        if (!itemData)
            continue;

        NBT::TagByte* tagSlotId = itemData->GetTagAs<NBT::TagByte>("Slot");
        if (!tagSlotId)
            continue;
        int slotId = tagSlotId->GetValue();

        if (slotId < 0 || slotId >= maxSlot)
            continue;

        ClearAndSetSlot(slotId, new ItemStack(itemData));
    }
}

} /* namespace Inventory */
