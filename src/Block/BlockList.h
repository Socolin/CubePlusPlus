#ifndef BLOCKLIST_H_
#define BLOCKLIST_H_

#include "Util/Singleton.h"

#include <stdint.h>
#include <map>

#include "BlockConstants.h"
#include "Block.h"
#include "Util/types.h"

namespace Block
{
/**
 * This class store all the blocks, it's a singleton so you can acces it anywhere
 * It load blocks' data, sound and material from database
 */
class BlockList : public Util::Singleton<BlockList>
{
    friend class Util::Singleton<BlockList>;
public:
    /**
     * Get block from blockid, blockId must be valid (0 < blockId < MAX_BLOCK_COUNT)
     * @param block blockId
     * @return
     */
    static const Block* getBlock(i_block blockId)
    {
        return Instance().blocks[blockId];
    }
    virtual ~BlockList();
    /**
     * Load blocks from database
     */
    void InitInstance() override;
private:
    BlockList();
    void Initialize();
    void LoadMaterials();
    void LoadSounds();
public:
    Block* blocks[BLOCK_COUNT];
    std::map<uint16_t, BlockMaterial> materialList;
    std::map<uint16_t, SoundBlock> soundList;
};

} /* namespace Block */
#endif /* BLOCKLIST_H_ */
