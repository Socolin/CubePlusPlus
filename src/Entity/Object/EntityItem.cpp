#include "EntityItem.h"
#include "Util/types.h"
#include "World/World.h"
#include "Block/Block.h"
#include "Block/BlockList.h"
#include "Network/OpcodeList.h"

namespace World
{

EntityItem::EntityItem(double x, double y, double z, Inventory::ItemStack itemStack, double motionX, double motionY, double motionZ)
    : Entity(ENTITY_TYPE_ITEM, x, y, z)
    , liveTime(0)
    , itemStack(itemStack)
{
    SetWidthHeight(0.25, 0.25);
    boundingBox.SetPositionCenteredXZ(x, y, z);
    this->motionX = motionX;
    this->motionY = motionY;
    this->motionZ = motionZ;
    metadataManager.SetEntityMetadata(10, itemStack);
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


    if (hasMove)
    {
        if (liveTime % 25 == 0)
        {
            // If in lava, "jump" and play sound

            // Check merge with item near
        }

        double slowDown = 0.98;
        if (onGround)
        {
            i_block blockBottomId = 0;
            if (1 < y && y < 256)
                blockBottomId = world->GetBlockId(floor(x), floor(y) - 1, floor(z));
            if (blockBottomId > 0)
            {
                Block::Block* blockBottom = Block::BlockList::getBlock(blockBottomId);
                if (blockBottom)
                {
                    slowDown = blockBottom->getSlipperiness() * 0.98f;
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
    if (liveTime >= 100)
        kill();

}

void EntityItem::GetSpecificUpdatePacket(Network::NetworkPacket& packet)
{
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
