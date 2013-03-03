#ifndef BLOCK_H_
#define BLOCK_H_

#include "Block/BlockMaterial.h"
#include "Block/SoundBlock.h"

namespace Util
{
class AABB;
}
namespace Scripting
{
class BlockScripts;
}
namespace Block
{
class EntityPlayer;
class World;

class Block
{
public:
    Block(unsigned short blockId,const SoundBlock& sound, unsigned char lightOpacity,
            unsigned char lightValue, float blockResistance, float blockHardness,
            bool needsRandomTick, float slipperiness,
            bool isCollidable, bool isOpaqueCube,
            const BlockMaterial& material, Scripting::BlockScripts* script);
    virtual ~Block();

    virtual void OnBlockPlace(World* world, EntityPlayer* player, int x, unsigned char y, int z, short& blockId, short& data);
    virtual void UpdateTick(World* world, int x, unsigned char y, int z, short blockId, short data);

    virtual const SoundBlock& GetSound() const;

private:
    unsigned short blockId;
    const SoundBlock sound;
    unsigned char lightOpacity;
    unsigned char lightValue;
    float blockResistance;
    float blockHardness;
    bool needsRandomTick;
    float slipperiness;
    bool isCollidable;
    bool isOpaqueCube;
    const BlockMaterial material;
    Scripting::BlockScripts* script;
};

} /* namespace Inventory */
#endif /* BLOCK_H_ */
