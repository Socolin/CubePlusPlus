#include "EntityHangingFrame.h"

#include "Inventory/ItemStack.h"
#include "Network/NetworkPacket.h"
#include "Network/OpcodeList.h"
#include "Entity/EntityPlayer.h"

namespace World
{

EntityHangingFrame::EntityHangingFrame(int x, i_height y, int z, int direction)
    : EntityHanging(ENTITY_TYPE_HANGINGFRAME,  x, y, z, direction)
{
    metadataManager.SetEntityMetadata(ENTITY_HANGING_FRAME_METADATA_ITEM, (Inventory::ItemStack*)nullptr);
    metadataManager.SetEntityMetadata(ENTITY_HANGING_FRAME_METADATA_ITEM_ROTATION, (char)0);
    setDirection(direction);
}

EntityHangingFrame::~EntityHangingFrame()
{
}

ItemUseResult EntityHangingFrame::Interact(EntityPlayer* player)
{
    const Inventory::ItemStack* itemContained = metadataManager.GetItemEntityMetadata(ENTITY_HANGING_FRAME_METADATA_ITEM);
    if (itemContained == nullptr)
    {
        Inventory::InventoryPlayer* handInventory = player->GetHandsInventory();
        const Inventory::ItemStack* usedItem = handInventory->LookSlot(handInventory->getHandSlotId());
        if (usedItem != nullptr)
        {
            Inventory::ItemStack* newItem = usedItem->Copy();
            newItem->setStackSize(1);
            metadataManager.SetEntityMetadata(ENTITY_HANGING_FRAME_METADATA_ITEM, newItem);
            metadataManager.SetEntityMetadata(ENTITY_HANGING_FRAME_METADATA_ITEM_ROTATION, (char)0);
        }
        return ItemUseResult{true, false, 1};
    }
    else
    {
        char oldOrientation = metadataManager.GetCharEntityMetadata(ENTITY_HANGING_FRAME_METADATA_ITEM_ROTATION);
        metadataManager.SetEntityMetadata(ENTITY_HANGING_FRAME_METADATA_ITEM_ROTATION, (char)((oldOrientation + 1) % 4));
    }
    return ItemUseResult{true, false, 0};
}

int EntityHangingFrame::getPixelWidth() const
{
    return 9;
}

int EntityHangingFrame::getPixelHeight() const
{
    return 9;
}

void EntityHangingFrame::GetCreatePacket(Network::NetworkPacket& packet)
{
    packet << (unsigned char) Network::OP_SPAWN_OBJECT_VEHICLE
            << entityId
            << (char) 71
            << (int)blockX * 32
            << (int)blockY * 32
            << (int)blockZ * 32
            << (char) (pitch * 256.f / 360.f)
            << (char) (yaw * 256.f / 360.f)
            << (int) direction;
    if (direction)
            packet << (short) 0 << (short) 0 << (short) 0;

    packet << (unsigned char) Network::OP_ENTITY_METADATA
            << entityId;
    metadataManager.Write(packet);
}

void EntityHangingFrame::GetSpecificUpdatePacket(Network::NetworkPacket& /*packet*/)
{
}

} /* namespace World */
