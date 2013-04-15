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
/**
 * This class represent a block in world, a block has some data stored in database
 * and can be associate to one script (in the future there will be possible to
 * associate more script) which define block behavior.
 */
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

    /**
     * Called when a block is place by a player, it call the script's method associated.
     * @param player The player that place block
     * @param x Position X of block
     * @param y Position Y of block
     * @param z Position Z of block
     * @param face The clicked face of block
     * @param blockId Id of the block
     * @param data Metadata of the block, it's a reference so it can be change
     * @param cursorPositionX Position of player cursor when he place block
     * @param cursorPositionY Position of player cursor when he place block
     * @param cursorPositionZ Position of player cursor when he place block
     */
    void OnBlockPlace(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const;

    /**
     * Called when a player right click on a block, it call the script's method associated.
     * @param user The player which do the right click
     * @param x Position X of block
     * @param y Position Y of block
     * @param z Position Z of block
     * @param face The clicked face of block
     * @param item The item stack that the player has in hand when he click, it's a reference
     * @param cursorPositionX Position of player cursor when he place block
     * @param cursorPositionY Position of player cursor when he place block
     * @param cursorPositionZ Position of player cursor when he place block
     * @return
     */
    bool UseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const;

    /**
     * This method is use for update a block, it can be call when block is in "to update list"
     * in chunk, added with MarkBlockForUpdate, or it can be call some random tick if
     * needsRandomTick is true, in this case, 3 block are called randomly each tick for each
     * chunks sections of 16x16x16.
     * It call the script's method associated.
     * @param world The world where the block is
     * @param x Position X of block
     * @param y Position Y of block
     * @param z Position Z of block
     * @param data The block metadata
     */
    void UpdateTick(World::World* world, int x, i_height y, int z, i_data data) const;

    /**
     * Called when a block is removed/replace by an other
     * It call the script's method associated.
     * @param world The world where the block is
     * @param x Position X of block
     * @param y Position Y of block
     * @param z Position Z of block
     * @param data The block metadata
     */
    void Destroy(World::World* world, int x, i_height y, int z, i_data data) const;

    /**
     * Used to try if a block can be place at the asking position
     * It call the script's method associated.
     * @param world The world where the block is
     * @param x Position X of block
     * @param y Position Y of block
     * @param z Position Z of block
     * @param face The clicked face
     * @return
     */
    bool CanPlace(World::World* world, int x, i_height y, int z, char face) const;

    /**
     * Called when a block as change in one of the six side of block
     * It call the script's method associated.
     * @param world The world where the block is
     * @param x Position X of block
     * @param y Position Y of block
     * @param z Position Z of block
     */
    void NeighborChange(World::World* world, int x, i_height y, int z) const;

    /**
     * Create a new TileEntity which will be associate to the block
     * @param blockX
     * @param blockY
     * @param blockZ
     * @return nullptr or an object that inherits from TileEntity
     */
    TileEntity* CreateNewTileEntity(int blockX, i_height blockY, int blockZ) const;

    /**
     * Check if the block use a tile entity or not
     * @return true if block use a tile entity, or false if not
     */
    bool UseTileEntity() const;

    /**
     * Fill the bbList with the different bounding box of the block
     * using minX/Y/Z and maxX/Y/Z, if a script is associated to this
     * block, it call the script's method associated.
     * @param x Position X of block
     * @param y Position Y of block
     * @param z Position Z of block
     * @param data The block metadata
     * @param bbList BoundingBox list
     */
    void GetBoundingBoxes(int x, int y, int z, i_data data, std::vector<Util::AABB>& bbList) const;

    /**
     * Return a basic bounding box using minX/Y/Z and maxX/Y/Z
     * @param x Position X of block
     * @param y Position Y of block
     * @param z Position Z of block
     * @return
     */
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
    /// The id of block.
    i_block blockId;

    // Database field.
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
