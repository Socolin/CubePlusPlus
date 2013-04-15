#include "Block.h"

#include <iostream>

#include "Util/AABB.h"
#include "SoundBlock.h"
#include "Scripts/BlockScript.h"

namespace Block
{


Block::~Block()
{
}

Block::Block(i_block blockId, const SoundBlock& sound, unsigned char lightOpacity,
             unsigned char lightValue, float blockResistance, float blockHardness, bool needsRandomTick,
             float slipperiness, bool isCollidable, bool isOpaqueCube, bool renderAsNormal,
             float minX, float minY, float minZ,
             float maxX, float maxY, float maxZ,
             const BlockMaterial& material,
             bool useNeighborBrightness,
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
    , useNeighborBrightness(useNeighborBrightness)
    , script(script)
{
    fullBlock = ((maxX - minX) + (maxZ - minZ) + (maxZ - minZ)) >= 3;
}

void Block::OnBlockPlace(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const
{
    if (script)
    {
        script->OnBlockPlacedBy(player, x, y, z, face, blockId, data, cursorPositionX, cursorPositionY, cursorPositionZ);
    }
}

void Block::UpdateTick(World::World* world, int x, i_height y, int z, i_data data) const
{
    if (script)
    {
        return script->OnUpdateTick(world, x, y, z, data);
    }
}

bool Block::CanPlace(World::World* world, int x, i_height y, int z, char face) const
{
	if (script)
    {
        return script->CanPlace(world, x, y, z, face);
	}
    return true;
}


bool Block::UseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, Inventory::ItemStack& item, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const
{
    if (script)
    {
        return script->OnUseBlock(user, x, y, z, face, item, cursorPositionX, cursorPositionY, cursorPositionZ);
    }
    return false;
}

TileEntity* Block::CreateNewTileEntity(int blockX, i_height blockY, int blockZ) const
{
    if (script)
    {
        return script->CreateNewTileEntity(blockX, blockY, blockZ);
    }
    return nullptr;
}

bool Block::UseTileEntity() const
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

void Block::GetBoundingBoxes(int x, int y, int z, i_data data, std::vector<Util::AABB>& bbList) const
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

void Block::NeighborChange(World::World* world, int x, i_height y, int z) const
{
    if (script)
    {
        script->OnNeighborChange(world, x, y, z);
    }
}

void Block::Destroy(World::World* world, int x, i_height y, int z, i_data data) const
{
    if (script)
    {
        script->OnDestroy(world, x, y, z, data);
    }
}

} /* namespace Block */
