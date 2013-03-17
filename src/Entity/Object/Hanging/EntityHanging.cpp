#include "EntityHanging.h"

#include "Block/Block.h"
#include "Block/BlockList.h"
#include "World/World.h"

namespace World
{

EntityHanging::EntityHanging(eEntityType type, int x, i_height y, int z, int direction)
    : Entity(type, x, y, z)
    , direction(direction)
    , blockX(x)
    , blockY(y)
    , blockZ(z)
    , updateTimer(0)
{
    SetWidthHeight(0.5, 0.5);
}

EntityHanging::~EntityHanging()
{
}

void EntityHanging::UpdateTick()
{
    if (dead)
        return;

    updateTimer++;
    if (updateTimer >= 100)
    {
        updateTimer = 0;

        if (!isOnValidSurface())
        {
            kill();
            // Drop
        }
    }
}

void EntityHanging::setDirection(int direction)
{
    this->direction = direction;
    yaw = direction * 90;
    double widthX = getPixelWidth();
    double height = getPixelHeight();
    double widthZ = getPixelWidth();

    if (direction != 2 && direction != 0)
    {
        widthX = 0.5F;
    }
    else
    {
        widthZ = 0.5F;
        yaw = (direction ^ 0x2) * 90;
    }

    widthX /= 32.0F;
    height /= 32.0F;
    widthZ /= 32.0F;
    double entityPositionX = (double)blockX + 0.5;
    double entityPositionY = (double)blockY + 0.5;
    double entityPositionZ = (double)blockZ + 0.5;
    double offset = 0.5625;

    if (direction == 2)
    {
        entityPositionZ -= offset;
    }

    if (direction == 1)
    {
        entityPositionX -= offset;
    }

    if (direction == 0)
    {
        entityPositionZ += offset;
    }

    if (direction == 3)
    {
        entityPositionX += offset;
    }

    if (direction == 2)
    {
        entityPositionX -= getWidthOffset(getPixelWidth());
    }

    if (direction == 1)
    {
        entityPositionZ += getWidthOffset(getPixelWidth());
    }

    if (direction == 0)
    {
        entityPositionX += getWidthOffset(getPixelWidth());
    }

    if (direction == 3)
    {
        entityPositionZ -= getWidthOffset(getPixelWidth());
    }

    entityPositionY += getWidthOffset(getPixelHeight());
    float boundingReduceSize = -0.03125F;

    x = entityPositionX;
    y = entityPositionY - (height + boundingReduceSize);
    z = entityPositionZ;

    boundingBox.SetWidthHeightDepth((widthX + boundingReduceSize) * 2,(height + boundingReduceSize) * 2,(widthZ + boundingReduceSize) * 2);
    boundingBox.SetPositionCenteredXYZ(entityPositionX, entityPositionY, entityPositionZ);
}

double EntityHanging::getWidthOffset(int width)
{
    if (width % 32 == 0)
        return 0.5;
    return 0.0;
}

bool EntityHanging::isOnValidSurface()
{
    std::vector<Util::AABB> bbList;
    world->GetBlockBoundingBoxInAABB(boundingBox, bbList);
    for (Util::AABB& box : bbList)
    {
        if (box.DetectCollision(boundingBox))
            return false;
    }

    int width = std::max(1, getPixelWidth() / 16);
    int height = std::max(1, getPixelHeight() / 16);
    int baseX = blockX;
    i_height baseY = blockY;
    int baseZ = blockZ;

    if (direction == 2)
    {
       baseX = std::floor(x - (double)getPixelWidth() / 32.0);
    }

    if (direction == 1)
    {
       baseZ = std::floor(z - (double)getPixelWidth() / 32.0);
    }

    if (direction == 0)
    {
       baseX = std::floor(x - (double)getPixelWidth() / 32.0);
    }

    if (direction == 3)
    {
       baseZ = std::floor(z - (double)getPixelWidth() / 32.0);
    }

    baseY = std::floor(y - (double)getPixelHeight() / 32.0);

    for (int blockXZ = 0; blockXZ < width; ++blockXZ)
    {
       for (int blockY = 0; blockY < height; ++blockY)
       {
           i_block blockId;

           if (direction != 2 && direction != 0)
           {
               blockId = world->GetBlockId(blockX, baseY + blockY, baseZ + blockXZ);
           }
           else
           {
               blockId = world->GetBlockId(baseX + blockXZ, baseY + blockY, blockZ);
           }
           if (blockId)
           {
               Block::Block* block = Block::BlockList::getBlock(blockId);
               if (block && !block->getMaterial().isSolid())
               {
                   return false;
               }
           }
           else
           {
               return false;
           }
       }
    }

    std::set<eEntityType> typeList;
    typeList.insert(ENTITY_TYPE_HANGINGFRAME);
    std::vector<std::pair<int, Util::AABB>> bbEntityList;
    world->GetEntitiesBoundingBoxInAABB(typeList, entityId, boundingBox, bbEntityList);

    return bbEntityList.empty();
}

} /* namespace World */
