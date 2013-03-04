#include "Block.h"

#include <iostream>

#include "Util/AABB.h"
#include "SoundBlock.h"
#include "Scripts/BlockScript.h"

namespace Block
{


Block::~Block()
{
    // TODO Auto-generated destructor stub
}

Block::Block(unsigned short blockId, const SoundBlock& sound, unsigned char lightOpacity,
             unsigned char lightValue, float blockResistance, float blockHardness, bool needsRandomTick,
             float slipperiness, bool isCollidable, bool isOpaqueCube, const BlockMaterial& material,
             Scripting::BlockScript* script)
    : blockId(blockId)
    , sound(sound)
    , lightOpacity(lightOpacity)
    , lightValue(lightValue)
    , blockResistance(blockResistance)
    , blockHardness(blockHardness)
    , needsRandomTick(needsRandomTick)
    , slipperiness(slipperiness)
    , isCollidable(isCollidable)
    , isOpaqueCube(isOpaqueCube)
    , material(material)
    , script(script)
{
}

void Block::OnBlockPlace(World::EntityPlayer* player, int x, unsigned char y, int z, int face, short& blockId, short& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
{
    if (script)
    {
        script->OnBlockPlacedBy(player, x, y, z, face, blockId, data, CursorpositionX, CursorpositionY, CursorpositionZ);
    }
}

void Block::UpdateTick(World::World* world, int x, unsigned char y, int z, short data)
{
}

bool Block::CanPlace(World::World* world, int x, unsigned char y, int z, short data)
{
    return true;
}

const SoundBlock& Block::GetSound() const
{
    return sound;
}

} /* namespace Inventory */
