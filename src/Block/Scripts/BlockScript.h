#ifndef BLOCKSCRIPTS_H_
#define BLOCKSCRIPTS_H_

#include <string>
#include "Util/types.h"

namespace Block
{
class Block;
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
    virtual void Init(Block::Block* baseBlock);

    virtual void InitParam(int paramId, int param) {}
    virtual void InitParam(int paramId, float param) {}
    virtual void InitParam(int paramId, const std::string& param) {}
    virtual void OnCreate() {};
    virtual bool CanPlace(World::World* world, int x, i_height y, int z, char face) {return true;}
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ) {}
    virtual bool OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ) {return false;}
    virtual BlockScript* Copy() = 0;
private:
    Block::Block* baseBlock;
};

} /* namespace Inventory */
#endif /* BLOCKSCRIPTS_H_ */
