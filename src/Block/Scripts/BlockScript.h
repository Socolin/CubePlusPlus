#ifndef BLOCKSCRIPTS_H_
#define BLOCKSCRIPTS_H_

#include <string>
#include <vector>

#include "Block/Block.h"
#include "Util/AABB.h"
#include "Util/types.h"

namespace Block
{
class Block;
class TileEntity;
}

namespace Inventory
{
class ItemStack;
}

namespace World
{
class EntityPlayer;
class World;
}

namespace Scripting
{

class BlockScript
{
public:
    BlockScript(const char* scriptName);
    virtual ~BlockScript();

    virtual BlockScript* Copy() = 0;

    virtual void Init(Block::Block* baseBlock);

    virtual void InitParam(int /*paramId*/, int /*param*/) {}
    virtual void InitParam(int /*paramId*/, float /*param*/) {}
    virtual void InitParam(int /*paramId*/, const std::string& /*param*/) {}

    /**
     * Call to know if a block can be place at the coordinate asked, if not return false, else return true
     * @param world world where block would be place
     * @param x block coordinate
     * @param y block coordinate
     * @param z block coordinate
     * @param face clicked face
     * @return true or false if block can be place
     */
    virtual bool CanPlace(World::World* world, int x, i_height y, int z, char face) const;

    /**
     * Called just before a block is place by player in world, blockId and data are reference, so it can be change
     * in this when block rotation depend of player orientation or where player has click
     * @param player the player whose place block
     * @param x position of block
     * @param y position of block
     * @param z position of block
     * @param face clicked face
     * @param blockId reference of future blockId
     * @param data reference of future block data
     * @param cursorPositionX position where player clicked on block for placing block
     * @param cursorPositionY position where player clicked on block for placing block
     * @param cursorPositionZ position where player clicked on block for placing block
     */
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const;

    /**
     * Called when a player right click on a block if something happen then return true, so item will not be use after
     * @param user player whose click on block
     * @param x position of clicked block
     * @param y position of clicked block
     * @param z position of clicked block
     * @param face clicked face
     * @param cursorPositionX position where player clicked on block
     * @param cursorPositionY position where player clicked on block
     * @param cursorPositionZ position where player clicked on block
     * @return true if something happen and nothing must be done after
     */
    virtual bool OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const;

    /**
     * Called after that a block has been place
     * @param world world where block has been added
     * @param x position of clicked block
     * @param y position of clicked block
     * @param z position of clicked block
     * @param data metadata of block
     */
    virtual void OnBlockAdded(World::World* world, int x, i_height y, int z, i_data data) const;

    /**
     * Return a new tile entity which will be store in chunk, to store more data, like for chest or
     * block with inventory
     * @param world current world
     * @param blockX position of block
     * @param blockY position of block
     * @param blockZ position of block
     * @return nullptr or valid a pointer to valid TileEntity
     */
    virtual Block::TileEntity* CreateNewTileEntity(World::World* world, int blockX, i_height blockY, int blockZ) const;

    /**
     * Indicate if block use a tile entity or not, if it return true, CreateNewTileEntity must not return nullptr
     * @return true if block has a tile entity
     */
    virtual bool UseTileEntity() const;


    /**
     * Called to update block
     * This is call X tick after calling World::MarkBlockForUpdate where X is the waitTick argument
     * This is also called randomly if block has needsRandomTick flag is true, in this case this method is call
     * approximately every 1 time every 1365 tick
     * @param world the world where block is
     * @param x position of block
     * @param y position of block
     * @param z position of block
     * @param data metadata of block
     */
    virtual void OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const;

    /**
     * Fill bbList with bounding boxes that compose block, example for entity npc
     * @param x position of block
     * @param y position of block
     * @param z position of block
     * @param data metadata of block
     * @param bbList reference to list that will be fill
     */
    virtual void GetBoundingBoxes(int x, int y, int z, i_data data, std::vector<Util::AABB>& bbList) const;

    /**
     * Called when a block on one of 6 faces has change
     * @param world the world where block is
     * @param x position of block
     * @param y position of block
     * @param z position of block
     * @param neighborBlockId id of block that has change
     */
    virtual void OnNeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const;

    /**
     * Called when block is remove from world
     * @param world the world where block is
     * @param x position of block
     * @param y position of block
     * @param z position of block
     * @param data metadat of block
     */
    virtual void OnDestroy(World::World* world, int x, i_height y, int z, i_data data) const;

    /**
     * Return true if block has the top surface solid, like for reversedb   stair/slab or glowstone
     * @param metadata metadata of block
     * @return true if top face is solid
     */
    virtual bool HasSolidTopSurface(i_data metadata) const;

    /**
     * Indicate if block can generate redstone signal
     * @return true if block can generate redstone
     */
    virtual bool CanProvidePower() const;

    virtual bool IsFullBlock() const;

    virtual i_powerlevel GetWeakPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const;
    virtual i_powerlevel GetStrongPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const;

    virtual void OnNotifyTileEntityStateChange(World::World* world, int x, i_height y, int z, int action);

protected:
    Block::Block* baseBlock;
};

} /* namespace Scripting */
#endif /* BLOCKSCRIPTS_H_ */
