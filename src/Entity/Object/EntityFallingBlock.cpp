#include "EntityFallingBlock.h"
#include "World/World.h"
#include "Network/OpcodeList.h"
#include "Inventory/ItemStack.h"

namespace World
{

EntityFallingBlock::EntityFallingBlock(double x, double y, double z, i_block blockId, i_data blockData)
    : Entity(ENTITY_TYPE_FALLINGBLOCK, x, y, z)
    , blockId(blockId)
    , blockData(blockData)
{
    SetWidthHeight(0.98, 0.98);
    boundingBox.SetPositionCenteredXZ(x, y, z);
}

EntityFallingBlock::~EntityFallingBlock()
{
}

void EntityFallingBlock::UpdateTick()
{
    motionY -= 0.03999999910593033;
    Move(0, motionY, 0);
    motionY *= 0.9800000190734863;
    if (onGround)
    {
        bool canPlaceBlock = true;
        i_block replaceBlockId = world->GetBlockId(std::floor(x), std::floor(y), std::floor(z));
        if (replaceBlockId)
        {
            const Block::Block* block = Block::BlockList::getBlock(replaceBlockId);
            if (block)
            {
                if (!block->GetMaterial().isReplacable())
                    canPlaceBlock = false;
            }
        }
        if (canPlaceBlock)
            world->ChangeBlock(std::floor(x), std::floor(y), std::floor(z), blockId, blockData, false);
        else
            world->DropItemstackWithRandomDirection(x, y, z, Inventory::ItemStack(blockId, 1, blockData));
        kill();
    }
}

void EntityFallingBlock::GetSpecificUpdatePacket(Network::NetworkPacket& packet)
{
}

void EntityFallingBlock::GetCreatePacket(Network::NetworkPacket& packet)
{
    packet << (unsigned char) Network::OP_SPAWN_OBJECT_VEHICLE
            << entityId
            << (char) 70 /*TODO:enum*/
            << networkX
            << networkY
            << networkZ
            << (char) (yaw * 256.f / 360.f)
            << (char) (pitch * 256.f / 360.f)
            << (unsigned int)(blockId| (blockData << 0x12))
            << (short)0 << (short)0 << (short)0;
}

} /* namespace World */
