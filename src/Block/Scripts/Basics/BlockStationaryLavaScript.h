#ifndef BLOCK_STATIONARY_LAVA_SCRIPT_H_
#define BLOCK_STATIONARY_LAVA_SCRIPT_H_

#include "Block/Scripts/Basics/BlockStationaryScript.h"

namespace Scripting
{
class BlockStationaryLavaScript: public BlockStationaryScript
{
    typedef BlockStationaryScript parent_type;
public:
    BlockStationaryLavaScript();
    virtual ~BlockStationaryLavaScript ();
    virtual BlockScript* Copy() override;
    virtual void OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const override;
private:
    bool isFlammable(World::World* world, int x, i_height y, int z) const;
};
 
} /* namespace Scripting */
#endif /* BLOCK_STATIONARY_LAVA_SCRIPT_H_ */
