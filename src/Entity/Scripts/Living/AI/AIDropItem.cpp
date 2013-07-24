#include "AIDropItem.h"

#include "Entity/Scripts/ScriptedLivingEntity.h"
#include "World/World.h"
#include "Util/FloatUtil.h"

namespace Scripting
{

AIDropItem::AIDropItem()
    : dropItemTimer(0)
    , dropItemMinTimer(6000)
    , dropItemMaxTimer(12000)
    , dropItemItemId(0)
    , dropItemItemData(0)
    , dropItemQuantity(1)
    , baseScript(nullptr)
{
}

AIDropItem::~AIDropItem()
{
}

void AIDropItem::DropItemInit(LivingEntityScript* script, i_item itemId, i_data itemData, i_stackSize itemQuantity, int minTimer, int maxTimer)
{
    baseScript = script;

    dropItemItemId = itemId;
    dropItemItemData = itemData;
    dropItemQuantity = itemQuantity;
    dropItemMinTimer = minTimer;
    dropItemMaxTimer = maxTimer;

    if (dropItemMaxTimer == dropItemMinTimer)
        dropItemTimer = dropItemMinTimer;
    else
        dropItemTimer = dropItemMinTimer + (rand() % (dropItemMaxTimer - dropItemMinTimer));
}

void AIDropItem::DropItemUpdate(World::LivingEntity* baseEntity)
{
    if (dropItemTimer <= 0)
    {
        if (dropItemMaxTimer == dropItemMinTimer)
            dropItemTimer = dropItemMinTimer;
        else
            dropItemTimer = dropItemMinTimer + (rand() % (dropItemMaxTimer - dropItemMinTimer));

        baseEntity->GetWorld()->DropItemstack(*baseEntity, new Inventory::ItemStack(dropItemItemId, dropItemQuantity, dropItemItemData));
        float soundModifier = ((Util::randFloat() - Util::randFloat()) * 0.2f) + 1.f;
        baseEntity->GetWorld()->PlaySound(baseEntity->x, baseEntity->y, baseEntity->z, L"mob.chicken.plop", 1.0f, soundModifier, 2);
    }
    else
        dropItemTimer--;
}

} /* namespace World */
