#ifndef BLOCKREDSTONEPOWERED_H_
#define BLOCKREDSTONEPOWERED_H_

#include "Block/Scripts/BlockScript.h"

namespace Scripting
{

/*
 * INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('block_redstonepowered',0);
 */
class BlockRedstonePowered : public BlockScript
{
public:
    BlockRedstonePowered();
    virtual ~BlockRedstonePowered();
    virtual BlockScript* Copy() override;
    virtual bool CanProvidePower() const override;
    virtual i_powerlevel GetWeakPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const override;
};

} /* namespace Scripting */
#endif /* BLOCKREDSTONEPOWERED_H_ */
