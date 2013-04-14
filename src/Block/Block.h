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
          bool renderAsNormal,
          float minX, float minY, float minZ,
          float maxX, float maxY, float maxZ,
          const BlockMaterial& material,
          bool useNeighborBrightness,
          Scripting::BlockScript* script);
    virtual ~Block();

    void InitScript();

    void OnBlockPlace(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const;
    bool UseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const;
    void UpdateTick(World::World* world, int x, i_height y, int z, i_data data) const;
    void Destroy(World::World* world, int x, i_height y, int z, i_data data) const;
    bool CanPlace(World::World* world, int x, i_height y, int z, char face) const;
    void NeighborChange(World::World* world, int x, i_height y, int z) const;

    TileEntity* CreateNewTileEntity() const;
    bool UseTileEntity() const;

    void GetBoundingBoxes(int x, int y, int z, i_data data, std::vector<Util::AABB>& bbList) const;
    const Util::AABB GetBoundingBox(int x, int y, int z) const;

    // Getters
    inline i_block GetBlockId() const;
    inline const SoundBlock& GetSound() const;
    inline const BlockMaterial& GetMaterial() const;
    inline bool IsOpaqueCube() const;
    inline bool IsFullBlock() const;
    inline bool NeedsRandomTick() const;
    inline bool IsRenderAsNormal() const;
    inline i_lightopacity GetLightOpacity() const;
    inline i_lightvalue GetLightValue() const;
    inline float GetSlipperiness() const;
    inline bool UseNeighborBrightness() const;

private:
    // Database data
    i_block blockId;
    const SoundBlock sound;
    i_lightopacity lightOpacity;
    i_lightvalue lightValue;
    float blockResistance;
    float blockHardness;
    bool needsRandomTick;
    float slipperiness;
    bool isCollidable;
    bool isOpaqueCube;
    bool renderAsNormal;
    float minX;
    float minY;
    float minZ;
    float maxX;
    float maxY;
    float maxZ;
    const BlockMaterial material;
    bool useNeighborBrightness;
    Scripting::BlockScript* script;

    // Calculated data
    bool fullBlock;
};

} /* namespace Block */

#include "Block.hxx"

#endif /* BLOCK_H_ */
