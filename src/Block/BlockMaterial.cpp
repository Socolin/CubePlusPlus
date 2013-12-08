#include "BlockMaterial.h"

namespace Block
{

BlockMaterial::BlockMaterial(unsigned char id, bool canBurn, bool replacable, bool translucent, bool requiresNoTool, int mobilityFlag, bool solid, bool liquid, bool blocksMovement)
    : id(id)
    , canBurn(canBurn)
    , replacable(replacable)
    , translucent(translucent)
    , requiresNoTool(requiresNoTool)
    , mobilityFlag(mobilityFlag)
    , solid(solid)
    , liquid(liquid)
    , blocksMovement(blocksMovement)
{

}

BlockMaterial::BlockMaterial()
    : id(0)
    , canBurn(false)
    , replacable(false)
    , translucent(false)
    , requiresNoTool(false)
    , mobilityFlag(0)
    , solid(true)
    , liquid(false)
    , blocksMovement(true)
{
}

BlockMaterial::~BlockMaterial()
{
}

} /* namespace Block */
