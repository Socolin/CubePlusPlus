#include "Block/Block.h"
#include "Block/BlockList.h"
#include "EntityItem.h"
#include "Entity/EntityPlayer.h"
#include "Network/OpcodeList.h"
#include "Util/FloatUtil.h"
#include "Util/types.h"
#include "Window/Window.h"
#include "World/World.h"
#include "World/VirtualSmallChunk.h"

namespace World
{

EntityItem::EntityItem(double x, double y, double z, Inventory::ItemStack* itemStack, double motionX, double motionY, double motionZ, int timeBeforePickup)
    : Entity(ENTITY_TYPE_ITEM, ENTITY_TYPEFLAG_MOVING, x, y, z)
    , liveTime(0)
    , timeBeforePickup(timeBeforePickup)
    , storedItem(1)
{
    SetWidthHeight(0.25, 0.25);
    boundingBox.SetPositionCenteredXZ(x, y, z);
    this->motionX = motionX;
    this->motionY = motionY;
    this->motionZ = motionZ;
    storedItem.ClearAndSetSlot(0, itemStack);
    metadataManager.SetEntityMetadata(10, itemStack->Copy());
}

EntityItem::~EntityItem()
{
}

void EntityItem::UpdateTick()
{
    if (dead)
        return;

    liveTime++;

    motionY -= 0.03999999910593033;

    // opti : if !noclip check every 5 tick ?
    noclip = PushOutOfBlock(x, y + (boundingBox.getHeight() / 2), z);

    Move(motionX, motionY, motionZ);

    // TODO:Maybe move it into 'if (hasmove) and reduce timer ?'
    if (liveTime % 25 == 0)
    {
        // If in lava, "jump" and play sound

        // Check merge with item near
        std::vector<Entity*> entityList;
        world->GetEntitiesInRangeByEntityType(ENTITY_TYPE_ITEM, entityId, *this, 1, entityList);
        for (Entity* entity : entityList)
        {
            if (entity->IsDead())
                continue;
            EntityItem* entityItem = dynamic_cast<EntityItem*>(entity);
            if (entityItem != nullptr)
            {
                const Inventory::ItemStack* lookedOtheItem = entityItem->storedItem.LookSlot(0);
                const Inventory::ItemStack* lookedOwnItem = storedItem.LookSlot(0);
                if (lookedOtheItem->IsStackable(lookedOwnItem))
                {
                    if (lookedOtheItem->getStackSize() + lookedOwnItem->getStackSize() <= lookedOwnItem->GetMaxStackSize())
                    {
                        Inventory::ItemStack* itemStack = entityItem->storedItem.TakeSlot(0);
                        storedItem.Merge(0, itemStack);
                        metadataManager.SetEntityMetadata(10, storedItem.LookSlot(0)->Copy());
                        entityItem->Kill();
                    }
                }
            }
        }
    }

    if (hasMove)
    {
        double slowDown = 0.98;
        if (onGround)
        {
            i_block blockBottomId = 0;
            if (1 < y && y < 256)
                blockBottomId = world->GetBlockId(floor(x), floor(y) - 1, floor(z));
            if (blockBottomId > 0)
            {
                const Block::Block* blockBottom = Block::BlockList::getBlock(blockBottomId);
                if (blockBottom)
                {
                    slowDown = blockBottom->GetSlipperiness() * 0.98f;
                }
                else
                    slowDown = 0.58800006;
            }
        }

        motionX *= slowDown;
        motionY *= 0.9800000190734863;
        motionZ *= slowDown;

        if (onGround)
        {
            motionY *= -0.5;
        }

        if (fabs(motionX) < 0.00001 && fabs(motionZ) < 0.00001)
        {
            motionX = 0;
            motionZ = 0;
        }
    }
    if (liveTime >= 6000)
        Kill();

}

void EntityItem::OnCollideWithPlayer(EntityPlayer* player)
{
    if (dead)
        return;
    if (liveTime < timeBeforePickup)
        return;
    Window::Window* inventoryWindow = player->GetInventoryWindow();
    const Inventory::ItemStack* lookedStoredItem = LookStoreItem();
    int count = inventoryWindow->CountAvaibleSpaceForItem(Inventory::INVENTORY_TYPE_PLAYER_HANDS | Inventory::INVENTORY_TYPE_PLAYER_MAIN, lookedStoredItem);
    if (count > 0)
    {
        Inventory::ItemStack* takenItem = nullptr;
        if (count >= lookedStoredItem->getStackSize())
        {
            takenItem = storedItem.TakeSlot(0);
            Kill();
        }
        else
        {
            takenItem = storedItem.TakeSomeItemInSlot(0, count);
            metadataManager.SetEntityMetadata(10, storedItem.LookSlot(0)->Copy());
        }
        VirtualSmallChunk* vSmallChunk = world->GetVirtualSmallChunkIfLoaded(chunkX, chunkZ);
        if (vSmallChunk)
        {
            Network::NetworkPacket collectItemPacket(Network::OP_COLLECT_ITEM);
            collectItemPacket << entityId << player->GetEntityId();
            vSmallChunk->SendPacketToAllNearPlayer(collectItemPacket);
        }
        world->PlaySound(x, y, z, L"random.pop", 0.2f, (char)(63.f * (((Util::randFloat() - Util::randFloat()) * 0.7f + 1.0f) * 2.0f)), 2);
        inventoryWindow->PlaceAllItemInStackToInventories(Inventory::INVENTORY_TYPE_PLAYER_HANDS | Inventory::INVENTORY_TYPE_PLAYER_MAIN,takenItem,false);
    }
}

void EntityItem::GetSpecificUpdatePacket(Network::NetworkPacket& packet)
{
    if (metadataManager.HasChanged())// Move it to entity class
    {
        metadataManager.ClearChange();
        packet << (unsigned char) Network::OP_ENTITY_METADATA
                << entityId;
        metadataManager.Write(packet);
    }
}

void EntityItem::GetCreatePacket(Network::NetworkPacket& packet)
{
    packet << (unsigned char) Network::OP_SPAWN_OBJECT_VEHICLE
            << entityId
            << (char) 2
            << networkX
            << networkY
            << networkZ
            << (char) (yaw * 256.f / 360.f)
            << (char) (pitch * 256.f / 360.f)
            << (int) 0;

    packet << (unsigned char) Network::OP_ENTITY_METADATA
            << entityId;
    metadataManager.Write(packet);
}

const Inventory::ItemStack* EntityItem::LookStoreItem() const
{
    return storedItem.LookSlot(0);
}

} /* namespace World */
