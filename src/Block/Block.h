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
class BlockScript;
}
namespace World
{
class EntityPlayer;
class World;
}
namespace Inventory
{
class ItemStack;
}
namespace Block
{

class Block
{
public:
    Block(unsigned short blockId,const SoundBlock& sound, unsigned char lightOpacity,
          unsigned char lightValue, float blockResistance, float blockHardness,
          bool needsRandomTick, float slipperiness,
          bool isCollidable, bool isOpaqueCube,
          const BlockMaterial& material, Scripting::BlockScript* script);
    virtual ~Block();

    virtual void OnBlockPlace(World::EntityPlayer* player, int x, unsigned char y, int z, int face, short& blockId, short& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ);
    virtual bool UseBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ);
    virtual void UpdateTick(World::World* world, int x, unsigned char y, int z, short data);
    virtual bool CanPlace(World::World* world, int x, unsigned char y, int z, short data);
    virtual const SoundBlock& GetSound() const;
    const BlockMaterial& getMaterial() const;

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
    Scripting::BlockScript* script;
};

} /* namespace Inventory */
#endif /* BLOCK_H_ */
