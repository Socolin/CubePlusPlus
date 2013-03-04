#include "BlockMaterial.h"

namespace Block
{

BlockMaterial::BlockMaterial(bool canBurn, bool replacable, bool translucent, bool requiresNoTool, int mobilityFlag)
    : canBurn(canBurn)
    , replacable(replacable)
    , translucent(translucent)
    , requiresNoTool(requiresNoTool)
    , mobilityFlag(mobilityFlag)
{

}

BlockMaterial::BlockMaterial() :
    canBurn(false), replacable(false), translucent(false), requiresNoTool(false), mobilityFlag(0)
{
}

BlockMaterial::~BlockMaterial()
{
    // TODO Auto-generated destructor stub
}

} /* namespace Scripting */
