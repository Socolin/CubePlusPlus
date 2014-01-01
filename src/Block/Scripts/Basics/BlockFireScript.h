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
    virtual bool CanPlace(World::World* world, int x, i_height y, int z, char face) const override;
    virtual void OnBlockAdded(World::World* world, int x, i_height y, int z, i_data data) const override;
    virtual void OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const override;
    virtual void OnNeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const override;
    void EnableSpread(bool spread);
    bool CanBlockCatchFire(World::World* world, int x, i_height y, int z) const;
    int GetChanceToEncourageFire(World::World* world, int x, i_height y, int z, int par5) const;
private:
    bool canNeighborBurn(World::World* world, int x, i_height y, int z) const;
    void tryToCatchBlockOnFire(World::World* world, int x, i_height y, int z, int par5, int randPar) const;
    int getChanceOfNeighborsEncouragingFire(World::World* world, int x, i_height y, int z) const;
    bool doFireTick;
};
 
} /* namespace Scripting */
#endif /* BLOCK_FIRE_SCRIPT_H_ */
