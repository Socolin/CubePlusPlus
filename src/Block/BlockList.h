#ifndef BLOCKLIST_H_
#define BLOCKLIST_H_

#include <stdint.h>
#include <map>

#include "BlockConstants.h"
#include "Block.h"

namespace Block
{
class BlockList
{
public:
    static void InitInstance()
    {
        instance = new BlockList();
        instance->Load();
    }
    static BlockList* Instance()
    {
        return instance;
    }

    static inline Block* getBlock(i_block block)
    {
        return instance->blocks[block];
    }
    void Load();
    void LoadSounds();
    void LoadMaterials();
private:
    BlockList();
    static BlockList* instance;
    virtual ~BlockList();
public:
    Block* blocks[BLOCK_COUNT];
    std::map<uint16_t, BlockMaterial> materialList;
    std::map<uint16_t, SoundBlock> soundList;
};

} /* namespace Inventory */
#endif /* BLOCKLIST_H_ */
