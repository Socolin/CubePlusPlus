#ifndef BLOCK_H_
#define BLOCK_H_

#include <vector>

#include "Util/types.h"
#include "Util/AABB.h"
#include "Block/BlockMaterial.h"
#include "Block/SoundBlock.h"

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

    void OnBlockPlace(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ);
    bool UseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ);
    void UpdateTick(World::World* world, int x, i_height y, int z, i_data data);
    bool CanPlace(World::World* world, int x, i_height y, int z, char face);
    const SoundBlock& GetSound() const;
    bool GetIsOpaqueCube();
    const BlockMaterial& getMaterial() const;
    TileEntity* CreateNewTileEntity();
    bool UseTileEntity();
    void InitScript();
    i_block GetBlockId() const;
    void GetBoundingBoxes(int x, int y, int z, i_data data, std::vector<Util::AABB>& bbList);
    float getSlipperiness() const {return slipperiness;}

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
