#ifndef BLOCK_HXX_
#define BLOCK_HXX_

namespace Block
{

i_block Block::GetBlockId() const
{
    return blockId;
}

const SoundBlock& Block::GetSound() const
{
    return sound;
}

const BlockMaterial& Block::GetMaterial() const
{
    return material;
}

bool Block::IsOpaqueCube() const
{
    return isOpaqueCube;
}

bool Block::IsEdgeAverageMoreThanOne() const
{
    return edgeAverageMoreThanOne;
}

bool Block::NeedsRandomTick() const
{
    return needsRandomTick;
}

bool Block::IsRenderAsNormal() const
{
    return renderAsNormal;
}

i_lightopacity Block::GetLightOpacity() const
{
    return lightOpacity;
}

i_lightvalue Block::GetLightValue() const
{
    return lightValue;
}

float Block::GetSlipperiness() const
{
    return slipperiness;
}

bool Block::UseNeighborBrightness() const
{
    return useNeighborBrightness;
}

int Block::GetBurningTime() const
{
    return burningTime;
}

} /* namespace Block */

#endif /* BLOCK_HXX_ */
