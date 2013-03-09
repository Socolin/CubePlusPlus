#include "BlockNoteScript.h"

#include "World/World.h"
#include "Block/BlockList.h"
#include "Block/BlockMaterial.h"
#include "Entity/EntityPlayer.h"
#include "Block/TileEntities/TileEntityNote.h"

namespace Scripting
{

BlockNoteScript::BlockNoteScript():
                BlockScript("block_note"),
                soundName({L"note.bd",L"note.snare",L"note.hat",L"note.bassattack",L"note.harp"})
{
}

BlockNoteScript::~BlockNoteScript()
{
}

BlockScript* BlockNoteScript::Copy()
{
    return new BlockNoteScript(*this);
}
bool BlockNoteScript::OnUseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
{
    World::World* world = user->getWorld();
    World::Chunk* chunk = world->GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk == nullptr)
        return true;
    Block::TileEntity* tileEntity = chunk->GetTileEntity(x & 0xf, y, z & 0xf);
    if (tileEntity)
    {
        Block::TileEntityNote* noteTileEntity = dynamic_cast<Block::TileEntityNote*>(tileEntity);
        if (noteTileEntity)
        {
            double note = noteTileEntity->nextNoteLevel();
            i_block bottomBlockId = chunk->getBlockAt(x & 0xf, y - 1, z & 0xf);
            Block::Block* bottomBlock = Block::BlockList::getBlock(bottomBlockId);
            int materialSoundId = 4;
            if (bottomBlock)
            {
                unsigned char bottomMaterialId = bottomBlock->getMaterial().getId();
                for (int i = 0; i < 4; i++)
                {
                    if (bottomMaterialId == materialId[i])
                    {
                        materialSoundId = i;
                        break;
                    }
                }
            }
            double pitch = std::pow(2.0, (note - 12.0) / 12.0);
            world->PlaySound(x + 0.5, y + 0.5, z + 0.5, soundName[materialSoundId], 3.f, pitch * 64, 3);
            world->PlayBlockAction(x, y, z, materialSoundId + 1, note, baseBlock->GetBlockId(), 2);
        }
    }
    return true;
}


Block::TileEntity* BlockNoteScript::CreateNewTileEntity()
{
    return new Block::TileEntityNote();
}

void BlockNoteScript::Init(Block::Block* baseBlock)
{
    BlockScript::Init(baseBlock);
    materialId[0] = Block::BlockList::getBlock(1)->getMaterial().getId(); // Material.rock
    materialId[1] = Block::BlockList::getBlock(12)->getMaterial().getId(); // Material.sand
    materialId[2] = Block::BlockList::getBlock(20)->getMaterial().getId(); // Material.glass
    materialId[3] = Block::BlockList::getBlock(5)->getMaterial().getId(); // Material.wood
    materialId[4] = 0;
}

bool BlockNoteScript::UseTileEntity()
{
    return true;
}

} /* namespace Scripting */