#ifndef BLOCKREDSTONETORCH_H_
#define BLOCKREDSTONETORCH_H_

#include "Block/Scripts/Basics/BlockTorchScript.h"

namespace Scripting
{

class BlockRedstoneTorch : public BlockTorchScript
{
    typedef BlockTorchScript parent_type;
public:
    BlockRedstoneTorch(const char* scriptName);
protected:
    bool isIndirectlyPowered(World::World* world, int x, int y, int z, i_data data) const;
    virtual bool CanProvidePower() const override;
};

} /* namespace Scripting */
#endif /* BLOCKREDSTONETORCH_H_ */
