#include "EntityItem.h"
#include "Util/types.h"
#include "World/World.h"
#include "Block/Block.h"
#include "Block/BlockList.h"
#include "Network/OpcodeList.h"

namespace World
{

EntityItem::EntityItem(double x, double y, double z, Inventory::ItemStack itemStack)
    : Entity(x, y, z)
    , liveTime(0)
    , itemStack(itemStack)
{

}

EntityItem::~EntityItem()
{
}

void EntityItem::UpdateTick()
{
    liveTime++;

    motionY -= 0.040;

    // If isInsideBlock
    //   noclip=true
    //   set motion XYZ to move out
    // else
    //   noclip = false

    Move(motionX, motionY, motionZ);

    if (hasMove && liveTime % 25 == 0)
    {
        // If in lava, "jump" and play sound

        // Check merge with item near
    }

    float slowDown = 0.98;
    i_block blockBottomId = 0;
    if (y > 1 && y < 255)
        blockBottomId = world->GetBlockId(x, y - 1, z);
    if (blockBottomId > 0)
    {
        Block::Block* blockBottom = Block::BlockList::getBlock(blockBottomId);
        if (blockBottom)
        {
            slowDown = blockBottom->getSlipperiness() * 0.98f;
        }
        else
            slowDown = 0.588f;
    }

    motionX *= slowDown;
    motionY *= 0.98f;
    motionZ *= slowDown;
}

void EntityItem::GetSpecificUpdatePacket(Network::NetworkPacket& packet)
{
}

void EntityItem::GetCreatePacket(Network::NetworkPacket& packet)
{
    packet << (unsigned char) Network::OP_SPAWN_DROPPED_ITEM
            << entityId
            << itemStack
            << networkX
            << networkY
            << networkZ
            << (char) (yaw * 256.f / 360.f)
            << (char) (pitch * 256.f / 360.f)
            << (char) 0;
    packet.dump();
}

} /* namespace World */
