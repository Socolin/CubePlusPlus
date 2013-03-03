#ifndef BLOCKSCRIPTS_H_
#define BLOCKSCRIPTS_H_

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

    virtual void OnCreate() {};
    virtual void OnBlockPlacedBy(World::EntityPlayer* player, unsigned char data, int x, unsigned char y, int z) {}
    virtual BlockScript* Copy() = 0;
private:
    Block::Block* baseBlock;
};

} /* namespace Inventory */
#endif /* BLOCKSCRIPTS_H_ */
