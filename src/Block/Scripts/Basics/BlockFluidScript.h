#ifndef BLOCK_FLUID_SCRIPT_H_
#define BLOCK_FLUID_SCRIPT_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

class BlockFluidScript: public BlockScript
{
public:
    BlockFluidScript();
    BlockFluidScript(const char* scriptName);
    virtual ~BlockFluidScript ();
    virtual BlockScript* Copy() override;
    virtual void OnBlockAdded(World::World* world, int x, i_height y, int z, i_data data) const override;
    virtual void OnNeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const override;
};
 
} /* namespace Scripting */
#endif /* BLOCK_FLUID_SCRIPT_H_ */
