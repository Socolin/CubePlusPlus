#include "EntityItem.h"
#include "Util/types.h"
#include "World/World.h"
#include "Block/Block.h"
#include "Block/BlockList.h"
#include "Network/OpcodeList.h"

namespace World
{

EntityItem::EntityItem(double x, double y, double z, Inventory::ItemStack itemStack, double motionX, double motionY, double motionZ)
    : Entity(x, y, z)
    , liveTime(0)
    , itemStack(itemStack)
{
    std::cout << y << std::endl;
    SetWidthHeight(0.25, 0.25);
    this->motionX = motionX;
    this->motionY = motionY;
    this->motionZ = motionZ;
}

EntityItem::~EntityItem()
{
}

void EntityItem::UpdateTick()
{
    liveTime++;

    motionY -= 0.03999999910593033;
//    std::cout << "motionY:" << motionY << std::endl;

    // If isInsideBlock
    //   noclip=true
    //   set motion XYZ to move out
    // else
    //   noclip = false

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
}

} /* namespace World */
