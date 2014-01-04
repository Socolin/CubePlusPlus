#ifndef BLOCK_FLOWING_SCRIPT_H_
#define BLOCK_FLOWING_SCRIPT_H_

#include "Block/Scripts/Basics/BlockFluidScript.h"

namespace Scripting
{

class BlockFlowingScript: public BlockFluidScript
{
    typedef BlockFluidScript parent_type;
public:
    BlockFlowingScript();
    virtual ~BlockFlowingScript ();
    virtual BlockScript* Copy() override;
    virtual void OnBlockAdded(World::World* world, int x, i_height y, int z, i_data data) const override;
    virtual void OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const override;
};
 
} /* namespace Scripting */
#endif /* BLOCK_FLOWING_SCRIPT_H_ */
