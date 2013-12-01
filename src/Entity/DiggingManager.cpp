#include "DiggingManager.h"

#include "Block/Block.h"
#include "EntityPlayer.h"
#include "Inventory/Item.h"
#include "World/World.h"

namespace World
{

DiggingManager::DiggingManager(EntityPlayer* player)
    : player(player)
    , diggingBlock(false)
    , playerFinishDigging(false)
    , dropAtEnd(false)
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
        if (playerFinishDigging && diggingProgress > 1.f)
        {
            playerFinishDigging = false;
            EndDigging();
        }
    }
}

void DiggingManager::StartDigging(int x, i_height y, int z)
{
    playerFinishDigging = false;
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

                if (block->GetMaterial().isRequiresNoTool())
                {
                    dropAtEnd = true;
                }
                else
                {
                    const Inventory::Item* item = player->LookItemInHand();
                    if (item)
                    {
                        dropAtEnd = item->CanHarvestBlock(blockId);
                    }
                }

                diggingStep = getDamageDonePerTickAgainstBlock(block);
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
            if (diggingProgress >= 1 - (diggingStep * 2)) // Avoid lag problem, so accept dig block in two less tick than normal
            {
                if (dropAtEnd)
                    player->GetWorld()->BreakBlock(x, y, z);
                else
                    player->GetWorld()->RemoveBlock(x, y, z);
                player->GetHandsInventory()->DamageItemInSlot(player->GetHandsInventory()->getHandSlotId(), 1);
            }
            else
            {
                playerFinishDigging = true;
                player->ResetBlock(x, y, z);
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

float DiggingManager::getDamageDonePerTickAgainstBlock(const Block::Block* block)
{
    float damageDone = block->GetDamageDonePerTickByItem(player->LookItemInHand());

    //TODO, enchantement (aqua afinity, efficiency...) , potion (digSlowdown, digSpeed), and water reduction
    if (!player->isOnGround())
        damageDone /= 5.f;

    return damageDone;
}

} /* namespace World */
