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

    virtual bool CanPlace(World::World* world, int x, i_height y, int z, char face) {return true;}
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ) {}
    virtual bool OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ) {return false;}
    virtual Block::TileEntity* CreateNewTileEntity() {return nullptr;}
    virtual bool UseTileEntity() {return false;}
    virtual void OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data);
    virtual void GetBoundingBoxes(int x, int y, int z, i_data data, std::vector<Util::AABB>& bbList) { bbList.push_back(baseBlock->GetBoundingBox(x, y, z)); }
    virtual void OnNeighborChange(World::World* world, int x, i_height y, int z) {}
    virtual void OnDestroy(World::World* world, int x, i_height y, int z, i_data data) {}
protected:
    Block::Block* baseBlock;
};

} /* namespace Inventory */
#endif /* BLOCKSCRIPTS_H_ */
