#include "Block.h"

#include <iostream>

#include "Util/AABB.h"
#include "SoundBlock.h"
#include "Scripts/BlockScript.h"
#include "Entity/EntityPlayer.h"
#include "World/World.h"
#include "Inventory/ItemStack.h"
#include "Inventory/Item.h"

namespace Block
{


Block::Block(i_block blockId, const SoundBlock& sound, i_lightopacity lightOpacity,
             i_lightvalue lightValue, float blockResistance, float blockHardness, bool needsRandomTick,
             float slipperiness, bool isCollidable, bool isOpaqueCube, bool renderAsNormal,
             float minX, float minY, float minZ,
             float maxX, float maxY, float maxZ,
             const BlockMaterial& material,
             bool useNeighborBrightness, int burningTime,
             Scripting::BlockScript* script,
             i_block replaceBlockId,
             short dropId, char dropDataMask, short dropCountMin, short dropCountMax)
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
    , burningTime(burningTime)
    , script(script)
    , replaceBlockId(replaceBlockId)
    , dropId(dropId)
    , dropDataMask(dropDataMask)
    , dropCountMin(dropCountMin)
    , dropCountMax(dropCountMax)
{
    edgeAverageMoreThanOne = ((maxX - minX) + (maxY - minY) + (maxZ - minZ)) >= 3;
}


Block::~Block()
{
    delete script;
}

void Block::OnBlockPlace(World::EntityPlayer* player, int x, i_height y, int z, int face, i_block& blockId, i_data& data, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const
{
    if (script)
    {
        script->OnBlockPlacedBy(player, x, y, z, face, blockId, data, cursorPositionX, cursorPositionY, cursorPositionZ);
    }
}

void Block::OnBlockAddedInWorld(World::World* world, int x, i_height y, int z, i_data data) const
{
    if (script)
    {
        script->OnBlockAdded(world, x, y, z, data);
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


ItemUseResult Block::UseBlock(World::EntityPlayer* user, int x, i_height y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const
{
    if (script)
    {
        return script->OnUseBlock(user, x, y, z, face, cursorPositionX, cursorPositionY, cursorPositionZ);
    }
    return ItemUseResult{false, false, 1};
}

TileEntity* Block::CreateNewTileEntity(World::World* world, int blockX, i_height blockY, int blockZ) const
{
    if (script)
    {
        return script->CreateNewTileEntity(world, blockX, blockY, blockZ);
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

void Block::NeighborChange(World::World* world, int x, i_height y, int z, i_block neighborBlockId) const
{
    if (script)
    {
        script->OnNeighborChange(world, x, y, z, neighborBlockId);
    }
}

void Block::Destroy(World::World* world, int x, i_height y, int z, i_data data) const
{
    if (script)
    {
        script->OnDestroy(world, x, y, z, data);
    }
}

bool Block::CanProvidePower() const
{
    if (script)
    {
        return script->CanProvidePower();
    }
    return false;
}


i_powerlevel Block::GetWeakPowerLevel(World::World* world, int x, i_height y, int z, int side) const
{
    i_data metadata = world->GetBlockData(x, y, z);
    i_powerlevel level = GetWeakPowerLevel(world, x, y, z, side, metadata);
    return level;
}

i_powerlevel Block::GetWeakPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const
{
    if (script)
    {
        return script->GetWeakPowerLevel(world, x, y, z, side, metadata);
    }
    return 0;
}

i_powerlevel Block::GetStrongPowerLevel(World::World* world, int x, i_height y, int z, int side) const
{
    i_data metadata = world->GetBlockData(x, y, z);
    i_powerlevel level = GetStrongPowerLevel(world, x, y, z, side, metadata);
    return level;
}

i_powerlevel Block::GetStrongPowerLevel(World::World* world, int x, i_height y, int z, int side, i_data metadata) const
{
    if (script)
    {
        return script->GetStrongPowerLevel(world, x, y, z, side, metadata);
    }
    return 0;
}

bool Block::HasSolidTopSurface(i_data metadata) const
{
    if (script)
    {
        return script->HasSolidTopSurface(metadata);
    }
    return !material.isTranslucent() && renderAsNormal;
}

bool Block::IsNormalCube() const
{
    return !material.isTranslucent() && renderAsNormal && !CanProvidePower();
}

void Block::NotifyTileEntityStateChange(World::World* world, int x, i_height y, int z, int action) const
{
    if (script)
    {
        return script->OnNotifyTileEntityStateChange(world, x, y, z, action);
    }
}

bool Block::IsFullBlock() const
{
    if (script)
    {
        return script->IsFullBlock();
    }
    return edgeAverageMoreThanOne;
}

void Block::Drop(World::World* world, int x, i_height y, int z) const
{
    if (dropId > 0)
    {
        int dropCount = 1;
        if (dropCountMax == dropCountMin)
        {
            dropCount = dropCountMin;
        }
        else
        {
           dropCount  = dropCountMin + (random() % (dropCountMax - dropCountMin));
        }

        if (dropCount > 0)
        {
            i_data metadata = world->GetBlockData(x, y, z);
            Inventory::ItemStack* itemStack = new Inventory::ItemStack(dropId, dropCount, metadata & dropDataMask);
            world->DropItemstackWithRandomDirection(x + 0.5, y + 0.5, z + 0.5, itemStack);
        }
    }
    else if (dropId < 0)
    {
        // TODO: Drop system, to add special loot, with random, different items etc...
    }
}

float Block::GetDamageDonePerTickByItem(const Inventory::Item* item) const
{
    float damageDone = 0.f;
    if (blockHardness == 0)
    {
        return 1000.0F;
    }
    else if (blockHardness > 0)
    {
        bool hasGoodToolForBlock = false;
        float strVsBlock = 1.0f;
        if (material.isRequiresNoTool())
        {
            hasGoodToolForBlock = true;
        }
        else
        {
            if (item)
            {
                hasGoodToolForBlock = item->CanHarvestBlock(blockId);
                strVsBlock = item->GetStrengthVsBlock(blockId);
            }
        }

        if (hasGoodToolForBlock)
        {
            damageDone = strVsBlock / blockHardness / 100.0F;
        }
        else
        {
            damageDone = strVsBlock / blockHardness / 30.0F;
        }
    }
    return damageDone;
}

} /* namespace Block */
