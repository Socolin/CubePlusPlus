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
class BlockList : public Util::Singleton<BlockList>
{
    friend class Util::Singleton<BlockList>;
public:
    static const Block* getBlock(i_block block)
    {
        return Instance().blocks[block];
    }
    virtual ~BlockList();
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
