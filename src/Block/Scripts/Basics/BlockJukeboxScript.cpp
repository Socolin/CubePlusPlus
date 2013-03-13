#include "BlockJukeboxScript.h"

#include "World/World.h"
#include "Block/BlockList.h"
#include "Block/BlockMaterial.h"
#include "Entity/EntityPlayer.h"
#include "Block/TileEntities/TileEntityRecordPlayer.h"

namespace Scripting
{

BlockJukeboxScript::BlockJukeboxScript():
                BlockScript("block_jukebox")
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
    {
        return false;
    }
    Block::TileEntity* tileEntity = chunk->GetTileEntity(x & 0xf, y, z & 0xf);
    if (tileEntity)
    {
        Block::TileEntityRecordPlayer* recordPlayer = dynamic_cast<Block::TileEntityRecordPlayer*>(tileEntity);

        if (recordPlayer && item_list.find(item.getItemId()) != item_list.end())
        {
            recordPlayer->SetRecord(item.getItemId());
            world->PlaySoundOrParticleEffect(x, y, z, 1005 /*TODO enum*/, item.getItemId(), false, 5);
            return true;
        }
    }
    return false;
}

Block::TileEntity* BlockJukeboxScript::CreateNewTileEntity()
{
    return new Block::TileEntityRecordPlayer();
}

bool BlockJukeboxScript::UseTileEntity()
{
    return true;
}

void BlockJukeboxScript::InitParam(int paramId, const std::string& param)
{
    if(paramId == 1)
    {
    	std::istringstream allowed_items(param);
    	int value;
    	for(std::string each; std::getline(allowed_items, each, ',');)
    	{
    		std::istringstream(each) >> value;
    		item_list.insert(value);
    	}
    }
    else
    {
    	std::cerr << "BAD PARAMETER ID: " << paramId << std::endl;
    }
}

} /* namespace Scripting */
