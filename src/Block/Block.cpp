#include "Block.h"

#include "Util/AABB.h"
#include "SoundBlock.h"

namespace Block
{


Block::~Block()
{
    // TODO Auto-generated destructor stub
}

Block::Block(unsigned short blockId, const SoundBlock& sound, unsigned char lightOpacity,
        unsigned char lightValue, float blockResistance, float blockHardness, bool needsRandomTick,
        float slipperiness, bool isCollidable, bool isOpaqueCube, const BlockMaterial& material,
        Scripting::BlockScripts* script)
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

void Block::OnBlockPlace(World* world, EntityPlayer* player, int x, unsigned char y, int z, short & blockId, short & data)
{
}

void Block::UpdateTick(World* world, int x, unsigned char y, int z, short blockId, short data)
{
}

const SoundBlock& Block::GetSound() const
{
    return sound;
}

} /* namespace Inventory */
