#ifndef BLOCK_STATIONARY_SCRIPT_H_
#define BLOCK_STATIONARY_SCRIPT_H_

#include "Block/Scripts/Basics/BlockFluidScript.h"

namespace Scripting
{

#define SCRIPTINGPARAM_BLOCK_STATIONARY_TICK_RATE 1

class BlockStationaryScript: public BlockFluidScript
{
    typedef BlockFluidScript parent_type;
public:
    BlockStationaryScript();
    BlockStationaryScript(const char* scriptName);
    virtual ~BlockStationaryScript ();
    virtual BlockScript* Copy() override;
    virtual void InitParam(int paramId, int param);
    virtual void OnNeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const override;
private:
    void setNotStationary(World::World* world, int x, i_height y, int z) const;
    int tickRate;
};
 
} /* namespace Scripting */
#endif /* BLOCK_STATIONARY_SCRIPT_H_ */
