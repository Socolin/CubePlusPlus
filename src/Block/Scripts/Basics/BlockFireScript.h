#ifndef BLOCK_FIRE_SCRIPT_H_
#define BLOCK_FIRE_SCRIPT_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

class BlockFireScript: public BlockScript
{
public:
    BlockFireScript();
    virtual ~BlockFireScript ();
    virtual BlockScript* Copy() override;
    virtual void Init(Block::Block* baseBlock) override;
    virtual void InitParam(int paramId, int param) override;
    virtual bool CanPlace(World::World* world, int x, i_height y, int z, char face) const override;
    virtual void OnBlockAdded(World::World* world, int x, i_height y, int z, i_data data) const override;
    virtual void OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const override;
};
 
} /* namespace Scripting */
#endif /* BLOCK_FIRE_SCRIPT_H_ */
