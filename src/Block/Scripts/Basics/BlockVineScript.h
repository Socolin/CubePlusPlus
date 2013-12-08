#ifndef BLOCK_VINE_SCRIPT_H_
#define BLOCK_VINE_SCRIPT_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

class BlockVineScript: public BlockScript
{
public:
    BlockVineScript();
    virtual ~BlockVineScript ();
    virtual BlockScript* Copy() override;
    virtual bool CanPlace(World::World* world, int x, i_height y, int z, char face) const override;
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const override;
    virtual void OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const override;
    virtual void GetBoundingBoxes(int x, int y, int z, i_data data, std::vector<Util::AABB>& bbList) const override;
    virtual void OnNeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const override;
private:
    bool CanPlaceOnBlock(i_block blockId) const;
    bool CanStay(World::World* world, int x, i_height y, int z) const;
};
 
} /* namespace Scripting */
#endif /* BLOCK_VINE_SCRIPT_H_ */
