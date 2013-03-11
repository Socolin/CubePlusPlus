#include "Block.h"

#include <iostream>

#include "Util/AABB.h"
#include "SoundBlock.h"
#include "Scripts/BlockScript.h"

namespace Block
{


Block::~Block()
{
    // TODO Auto-generated destructor stub
}

Block::Block(i_block blockId, const SoundBlock& sound, unsigned char lightOpacity,
             unsigned char lightValue, float blockResistance, float blockHardness, bool needsRandomTick,
             float slipperiness, bool isCollidable, bool isOpaqueCube, bool renderAsNormal,
             float minX, float minY, float minZ,
             float maxX, float maxY, float maxZ,
             const BlockMaterial& material,
             Scripting::BlockScript* script)
    : blockId(blockId)
    , sound(sound)
    , lightOpacity(lightOpacity)
    , lightValue(lightValue)
    , blockResistance(blockResistance)
    , blockHardness(blockHardness)
    , needsRandomTick(needsRandomTick)
    , slipperiness(slipperiness)
    , isCollidable(isCollidable)
    , isOpaqueCube(isOpaqueCube)
    , renderAsNormal(renderAsNormal)
    , minX(minX)
    , minY(minY)
    , minZ(minZ)
    , maxX(maxX)
    , maxY(maxY)
    , maxZ(maxZ)
    , material(material)
    , script(script)
{
}

void Block::OnBlockPlace(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
{
    if (script)
    {
        script->OnBlockPlacedBy(player, x, y, z, face, blockId, data, CursorpositionX, CursorpositionY, CursorpositionZ);
    }
}

void Block::UpdateTick(World::World* world, int x, i_height y, int z, i_data data)
{
}

bool Block::CanPlace(World::World* world, int x, i_height y, int z, char face)
{
	if (script)
    {
        return script->CanPlace(world, x, y, z, face);
	}
    return true;
}

const SoundBlock& Block::GetSound() const
{
    return sound;
}

bool Block::UseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
{
    if (script)
    {
        return script->OnUseBlock(user, x, y, z, face, item, CursorpositionX, CursorpositionY, CursorpositionZ);
    }
    return false;
}

bool Block::GetIsOpaqueCube()
{
    return isOpaqueCube;
}

const BlockMaterial& Block::getMaterial() const
{
    return material;
}

TileEntity* Block::CreateNewTileEntity()
{
    if (script)
    {
        return script->CreateNewTileEntity();
    }
    return nullptr;
}

bool Block::UseTileEntity()
{
    if (script)
    {
        return script->UseTileEntity();
    }
    return false;
}

void Block::InitScript()
{
    if (script)
    {
        return script->Init(this);
    }
}

i_block Block::GetBlockId() const
{
    return blockId;
}

void Block::GetBoundingBoxes(int x, int y, int z, i_data data, std::vector<Util::AABB>& bbList)
{
    if (script)
    {
        script->GetBoundingBoxes(x, y, z, data, bbList);
    }
    else
    {
        bbList.push_back(GetBoundingBox(x, y, z));
    }
}

const Util::AABB Block::GetBoundingBox(int x, int y, int z) const
{
    return Util::AABB(x + minX, y + minY, z + minZ, maxX - minX, maxY - minY, maxZ - minZ);
}


} /* namespace Inventory */
