#include "BlockFireScript.h"

#include "Block/Block.h"
#include "Block/BlockList.h"
#include "World/World.h"


namespace Scripting
{

BlockFireScript::BlockFireScript() :
    BlockScript("block_fire"),
    doFireTick(true)
{
}

BlockFireScript::~BlockFireScript()
{
}

BlockScript* BlockFireScript::Copy()
{
    return new BlockFireScript(*this);
}

bool BlockFireScript::CanPlace(World::World* world, int x, i_height y, int z, char /*face*/) const
{
    s_block_data bottomBlockData = world->GetBlockIdAndData(x, y - 1, z);
    const Block::Block* bottomBlock = Block::BlockList::getBlock(bottomBlockData.blockId);
    if(bottomBlock != nullptr)
    {
        return (bottomBlock->HasSolidTopSurface(bottomBlockData.blockData) || canNeighborBurn(world, x, y, z));
    }
    return false;
}

void BlockFireScript::OnBlockAdded(World::World* /*world*/, int /*x*/, i_height /*y*/, int /*z*/, i_data /*data*/) const
{
    //TODO : manage nether portal
}

void BlockFireScript::OnUpdateTick(World::World* world, int x, i_height y, int z, i_data data) const
{
    if(doFireTick)
    {
        i_block bottomBlockId = world->GetBlockId(x, y - 1, z);
        bool isBottomBlockFlammable = (bottomBlockId == 87); //FIXME : Remove magic number ?

        //TODO : If world is End and bottom block is bedrock, isBottomBlockFlammable = true

        if(!CanPlace(world, x, y, z, 0))
        {
            world->ChangeBlock(x, y, z, 0, 0, false);
        }

        //TODO : Rain extinguish fire

        else
        {
            if(data < 15)
            {
                world->ChangeDataNotify(x, y, z, data + (rand() % 3) / 2); // 1 in 3 chance to add 1 to block data
            }

            world->ChangeBlockNoEvent(x, y, z, baseBlock->GetBlockId(), 30 + (rand() % 10));
            if(!isBottomBlockFlammable && !canNeighborBurn(world, x, y, z))
            {
                s_block_data bottomBlockData = world->GetBlockIdAndData(x, y - 1, z);
                const Block::Block* bottomBlock = Block::BlockList::getBlock(bottomBlockData.blockId);
                if(bottomBlock != nullptr)
                {
                    if(!bottomBlock->HasSolidTopSurface(bottomBlockData.blockData) || data > 3)
                    {
                        world->ChangeBlock(x, y, z, 0, 0, false);
                    }
                }
            }
            else if(!isBottomBlockFlammable && !CanBlockCatchFire(world, x, y - 1, z) && data == 15 && (rand() % 4) == 0)
            {
                world->ChangeBlock(x, y, z, 0, 0, false);
            }
            else
            {
                bool isBlockHighHumidity = false; //TODO : Check if biome has high humidity
                int highHumidityMalus = 0;
                if(isBlockHighHumidity)
                {
                    highHumidityMalus = -50;
                }

                tryToCatchBlockOnFire(world, x + 1, y, z, 300 + highHumidityMalus, data);
                tryToCatchBlockOnFire(world, x - 1, y, z, 300 + highHumidityMalus, data);
                tryToCatchBlockOnFire(world, x, y - 1, z, 250 + highHumidityMalus, data);
                tryToCatchBlockOnFire(world, x, y + 1, z, 250 + highHumidityMalus, data);
                tryToCatchBlockOnFire(world, x, y, z - 1, 300 + highHumidityMalus, data);
                tryToCatchBlockOnFire(world, x, y, z + 1, 300 + highHumidityMalus, data);

                for(int xLoop = x - 1; xLoop <= x + 1; ++xLoop)
                {
                    for(int zLoop = z - 1; zLoop <= z + 1; ++zLoop)
                    {
                        for(int yLoop = y - 1; yLoop <= y + 4; ++yLoop)
                        {
                            if(xLoop != x || yLoop != y || zLoop != z)
                            {
                                int randomSpreadValue = 100;
                                if(yLoop > (y + 1))
                                {
                                    randomSpreadValue += (yLoop - (y + 1)) * 100;
                                }

                                int chanceOfNeighborsEncouragingFire = getChanceOfNeighborsEncouragingFire(world, xLoop, yLoop, zLoop);
                                if(chanceOfNeighborsEncouragingFire > 0)
                                {
                                    int chanceToSpreadFire = (chanceOfNeighborsEncouragingFire + 40 + world->GetGameType() * 7) / (data + 30);
                                    if(isBlockHighHumidity)
                                    {
                                        chanceToSpreadFire /= 2;
                                    }

                                    if(chanceToSpreadFire > 0 && (rand() % randomSpreadValue) <= chanceToSpreadFire) //TODO : Check if weather = not raining
                                    {
                                        int burningTime = data + (rand() % 5) / 4;
                                        if(burningTime > 15)
                                        {
                                            burningTime = 15;
                                        }
                                        world->ChangeBlock(xLoop, yLoop, zLoop, baseBlock->GetBlockId(), burningTime, false);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void BlockFireScript::OnNeighborChange(World::World* world, int x, i_height y, int z, i_block /*neighborBlockId*/) const
{
    s_block_data bottomBlockData = world->GetBlockIdAndData(x, y - 1, z);
    const Block::Block* bottomBlock = Block::BlockList::getBlock(bottomBlockData.blockId);
    if(bottomBlock != nullptr)
    {
        if(!bottomBlock->HasSolidTopSurface(bottomBlockData.blockData) || !canNeighborBurn(world, x, y, z))
        {
            world->ChangeBlock(x, y, z, 0, 0, false);
        }
    }
}

void BlockFireScript::EnableSpread(bool spread)
{
    doFireTick = spread;
}

bool BlockFireScript::canNeighborBurn(World::World* world, int x, i_height y, int z) const
{
    return CanBlockCatchFire(world, x + 1, y, z)?true:(CanBlockCatchFire(world, x - 1, y, z)?true:(CanBlockCatchFire(world, x, y - 1, z)?true:(CanBlockCatchFire(world, x, y + 1, z)?true:(CanBlockCatchFire(world, x, y, z - 1)?true:CanBlockCatchFire(world, x, y, z + 1)))));
}

bool BlockFireScript::CanBlockCatchFire(World::World* world, int x, i_height y, int z) const
{
    i_block blockId = world->GetBlockId(x, y, z);
    const Block::Block* block = Block::BlockList::getBlock(blockId);
    if(block != nullptr)
    {
        return (block->GetAbilityToCatchFire() > 0);
    }
    return false;
}

void BlockFireScript::tryToCatchBlockOnFire(World::World* world, int x, i_height y, int z, int randValue, int randBonus) const
{
    i_block blockId = world->GetBlockId(x, y, z);
    const Block::Block* block = Block::BlockList::getBlock(blockId);
    if(block != nullptr)
    {
        int abilityToCatchFire = block->GetAbilityToCatchFire();
        if((rand() % randValue) < abilityToCatchFire)
        {
            bool isTNT = world->GetBlockId(x, y, z) == 46; //FIXME : Remove magic number ?
            //TODO : Assert CanLightningStrikeAt(x, y, z)
            if((rand() % (randBonus + 10)) < 5)
            {
                int burningTime = randBonus + (rand() % 5) / 4;
                if(burningTime > 15)
                {
                    burningTime = 15;
                }
                world->ChangeBlock(x, y, z, baseBlock->GetBlockId(), burningTime, false);
            }
            else
            {
                world->ChangeBlock(x, y, z, 0, 0, false);
            }

            if(isTNT)
            {
                block->OnBlockDestroyedByFire(world, x, y, z);
            }
        }
    }
}

int BlockFireScript::getChanceOfNeighborsEncouragingFire(World::World* world, int x, i_height y, int z) const
{
    if(world->GetBlockId(x,y,z) != 0)
    {
        return 0;
    }
    else
    {
        int chance = GetChanceToEncourageFire(world, x + 1, y, z, 0);
        chance = GetChanceToEncourageFire(world, x - 1, y, z, chance);
        chance = GetChanceToEncourageFire(world, x, y - 1, z, chance);
        chance = GetChanceToEncourageFire(world, x, y + 1, z, chance);
        chance = GetChanceToEncourageFire(world, x, y, z - 1, chance);
        chance = GetChanceToEncourageFire(world, x, y, z + 1, chance);
        return chance;
    }
}

int BlockFireScript::GetChanceToEncourageFire(World::World* world, int x, i_height y, int z, int minChance) const
{
    i_block blockId = world->GetBlockId(x, y, z);
    const Block::Block* block = Block::BlockList::getBlock(blockId);
    if(block != nullptr)
    {
        int changeToEncourageFire = block->GetChanceToEncourageFire();
        return changeToEncourageFire > minChance ? changeToEncourageFire : minChance;
    }
    return minChance;
}

} /* namespace Scripting */
