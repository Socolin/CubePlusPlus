#ifndef BLOCK_H_
#define BLOCK_H_

#include "Util/types.h"
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
class TileEntity;
class Block
{
public:
    Block(i_block blockId,const SoundBlock& sound, unsigned char lightOpacity,
          unsigned char lightValue, float blockResistance, float blockHardness,
          bool needsRandomTick, float slipperiness,
          bool isCollidable, bool isOpaqueCube,
          const BlockMaterial& material, Scripting::BlockScript* script);
    virtual ~Block();

    virtual void OnBlockPlace(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ);
    virtual bool UseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ);
    virtual void UpdateTick(World::World* world, int x, i_height y, int z, i_data data);
    virtual bool CanPlace(World::World* world, int x, i_height y, int z, char face);
    virtual const SoundBlock& GetSound() const;
    virtual bool GetIsOpaqueCube();
    const BlockMaterial& getMaterial() const;
    TileEntity* CreateNewTileEntity();
    bool UseTileEntity();
    void InitScript();
    i_block GetBlockId();

private:
    i_block blockId;
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
