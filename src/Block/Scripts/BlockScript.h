#ifndef BLOCKSCRIPTS_H_
#define BLOCKSCRIPTS_H_

#include <string>

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
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, int x, unsigned char y, int z, int face, short& blockId, short& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ) {}
    virtual bool OnUseBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ) {return false;}
    virtual BlockScript* Copy() = 0;
private:
    Block::Block* baseBlock;
};

} /* namespace Inventory */
#endif /* BLOCKSCRIPTS_H_ */
