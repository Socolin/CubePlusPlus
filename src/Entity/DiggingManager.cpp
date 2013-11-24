#include "DiggingManager.h"

#include "EntityPlayer.h"
#include "World/World.h"

namespace World
{

DiggingManager::DiggingManager(EntityPlayer* player)
    : player(player)
    , diggingBlock(false)
    , diggingStep(0)
    , diggingProgress(0)
    , x(0)
    , y(0)
    , z(0)
{
}

DiggingManager::~DiggingManager()
{
}

void DiggingManager::Update()
{
    if (diggingBlock)
    {
        diggingProgress += diggingStep;
        if (diggingProgress > 1.f)
        {
            EndDigging();
        }
    }
}

void DiggingManager::StartDigging(int x, i_height y, int z)
{
    if (player->GetWorld()->isReadOnly())
    {
        player->ResetBlock(x, y, z);
    }
    else
    {
        if (player->GetGameMode() == EntityPlayer::GAMEMODE_CREATVE)
        {
            player->GetWorld()->RemoveBlock(x, y, z);
        }
        else
        {
            i_block blockId = player->GetWorld()->GetBlockId(x, y, z);
            const Block::Block* block = Block::BlockList::getBlock(blockId);
            if (block)
            {
                this->x = x;
                this->y = y;
                this->z = z;
                diggingBlock = true;
                diggingStep = player->getDamageDonePerTickAgainstBlock(block);
                if (diggingStep > 1.f)
                {
                    EndDigging();
                }
            }
        }
    }
}

void DiggingManager::EndDigging()
{
    if (player->GetWorld()->isReadOnly() || player->GetGameMode() == EntityPlayer::GAMEMODE_ADVENTURE)
    {
        player->ResetBlock(x, y, z);
    }
    else
    {
        if (player->GetGameMode() == EntityPlayer::GAMEMODE_CREATVE)
        {
            player->GetWorld()->RemoveBlock(x, y, z);
        }
        else if (diggingBlock)
        {
            if (diggingProgress >= 1 - (diggingProgress * 2)) // Avoid lag problem, so accept dig block in two less tick than normal
            {
                player->GetWorld()->BreakBlock(x, y, z);
            }
            else
            {
                return;
            }
        }
        else
        {
            player->ResetBlock(x, y, z);
        }
    }
    StopDigging();
}

void DiggingManager::StopDigging()
{
    diggingBlock = false;
    diggingProgress = 0;
    diggingStep = 0;
}

} /* namespace World */
