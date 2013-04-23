#ifndef CACTUSSCRIPT_H_
#define CACTUSSCRIPT_H_

#include "Block/Scripts/BlockScript.h"
#include <set>

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_cactus',1);
 * SET @scriptId = LAST_INSERT_ID();
 * INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,1,'allowed_to_grow_on',0);
 */
class BlockCactusScript : public BlockScript
{
public:
    BlockCactusScript();
    virtual ~BlockCactusScript();
    virtual BlockScript* Copy() override;
    virtual bool CanPlace(World::World* world, int x, i_height y, int z, char face) const override;
    virtual void OnNeighborChange(World::World* world, int x, i_height y, int z) const override;
    void OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const;
    virtual void GetBoundingBoxes(int x, int y, int z, i_data data, std::vector<Util::AABB>& bbList) const override;
    virtual void InitParam(int paramId, const std::string& param) override;

    bool CheckSideBySideBlocks(World::World* world, int x, i_height y, int z) const;
    std::set<i_block> allowed_to_grow_on;
};

} /* namespace Scripting */
#endif /* CACTUSSCRIPT_H_ */
