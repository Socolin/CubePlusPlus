#ifndef BLOCK_STATIONARY_SCRIPT_H_
#define BLOCK_STATIONARY_SCRIPT_H_

#include "Block/Scripts/Basics/BlockFluidScript.h"

namespace Scripting
{

class BlockStationaryScript: public BlockFluidScript
{
    typedef BlockFluidScript parent_type;
public:
    BlockStationaryScript();
    virtual ~BlockStationaryScript ();
    virtual BlockScript* Copy() override;
    virtual void OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const override;
    virtual void OnNeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const override;
};
 
} /* namespace Scripting */
#endif /* BLOCK_STATIONARY_SCRIPT_H_ */
