#include "EntityItem.h"
#include "Util/types.h"
#include "World/World.h"
#include "Block/Block.h"
#include "Block/BlockList.h"
#include "Network/OpcodeList.h"

namespace World
{

EntityItem::EntityItem(double x, double y, double z, Inventory::ItemStack* itemStack, double motionX, double motionY, double motionZ)
    : Entity(ENTITY_TYPE_ITEM, ENTITY_TYPEFLAG_MOVING, x, y, z)
    , liveTime(0)
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

    if (liveTime % 25 == 0)
    {
        // If in lava, "jump" and play sound

        // Check merge with item near
        std::vector<Entity*> entityList;
        world->GetEntitiesInRangeByEntityType(ENTITY_TYPE_ITEM, entityId, *this, 1, entityList);
        for (Entity* entity : entityList)
        {
            if (entity->isDead())
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
                        entityItem->kill();
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
        kill();

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

} /* namespace World */
