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

    virtual void InitParam(int paramId, int param) {}
    virtual void InitParam(int paramId, float param) {}
    virtual void InitParam(int paramId, const std::string& param) {}

    virtual bool CanPlace(World::World* world, int x, i_height y, int z, char face) const;
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const;
    virtual bool OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const;
    virtual void OnBlockAdded(World::World* world, int x, i_height y, int z, i_data data) const;
    virtual Block::TileEntity* CreateNewTileEntity(int blockX, i_height blockY, int blockZ) const;
    virtual bool UseTileEntity() const;
    virtual void OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const;
    virtual void GetBoundingBoxes(int x, int y, int z, i_data data, std::vector<Util::AABB>& bbList) const;
    virtual void OnNeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const;
    virtual void OnDestroy(World::World* world, int x, i_height y, int z, i_data data) const;
    virtual bool HasSolidTopSurface(i_data metadata) const;

    virtual bool CanProvidePower() const;
    virtual i_powerlevel GetWeakPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const;
    virtual i_powerlevel GetStrongPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const;

protected:
    Block::Block* baseBlock;
};

} /* namespace Scripting */
#endif /* BLOCKSCRIPTS_H_ */
