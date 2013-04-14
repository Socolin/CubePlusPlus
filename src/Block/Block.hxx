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

bool Block::IsFullBlock() const
{
    return fullBlock;
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

} /* namespace Block */

#endif /* BLOCK_HXX_ */
