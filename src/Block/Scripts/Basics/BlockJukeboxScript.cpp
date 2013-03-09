#include "BlockJukeboxScript.h"

#include "World/World.h"
#include "Block/BlockList.h"
#include "Block/BlockMaterial.h"
#include "Entity/EntityPlayer.h"
#include "Block/TileEntities/TileEntityRecordPlayer.h"

namespace Scripting
{

BlockJukeboxScript::BlockJukeboxScript():
                BlockScript("block_jukebox")//,
                //soundName({L"note.bd",L"note.snare",L"note.hat",L"note.bassattack",L"note.harp"})
{
}

BlockJukeboxScript::~BlockJukeboxScript()
{
}

BlockScript* BlockJukeboxScript::Copy()
{
    return new BlockJukeboxScript(*this);
}
bool BlockJukeboxScript::OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
{
    World::World* world = user->getWorld();
    World::Chunk* chunk = world->GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk == nullptr)
        return true;
    Block::TileEntity* tileEntity = chunk->GetTileEntity(x & 0xf, y, z & 0xf);
    if (tileEntity)
    {
        Block::TileEntityRecordPlayer* recordPlayer = dynamic_cast<Block::TileEntityRecordPlayer*>(tileEntity);
        if (recordPlayer)
        {
            recordPlayer->SetRecord(item.getItemId());
        }
    }
    return true;
}

} /* namespace Scripting */
