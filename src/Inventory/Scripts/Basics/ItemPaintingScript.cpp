#include "ItemPaintingScript.h"

#include "Block/BlockConstants.h"
#include "Entity/Object/Hanging/EntityPainting.h"
#include "Entity/EntityPlayer.h"
#include "Util/AssertUtil.h"
#include "World/World.h"

namespace Scripting
{

ItemPaintingScript::ItemPaintingScript()
    : ItemScript("item_painting")
{
}


ItemPaintingScript::~ItemPaintingScript()
{
}

ItemScript* ItemPaintingScript::Copy()
{
    return new ItemPaintingScript(*this);
}


World::ItemUseResult ItemPaintingScript::OnUseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    Inventory::InventoryPlayer* handInventory = user->GetHandsInventory();
    const Inventory::ItemStack* item = handInventory->LookSlot(handInventory->getHandSlotId());
    if (item == nullptr)
        return World::ItemUseResult{false, false, 0};

    int orientation = -1;
    switch (face)
    {
    case FACE_BOTTOM: // -Y
    case FACE_TOP: // +Y
    case FACE_NONE:
        return World::ItemUseResult{false, false, 0};
    case FACE_NORTH: // -Z
        orientation = 2;
        break;
    case FACE_SOUTH: // +Z
        orientation = 0;
        break;
    case FACE_WEST: // -X
        orientation = 1;
        break;
    case FACE_EAST: // +X
        orientation = 3;
        break;
    default:
        AssertSwitchBadDefault(face)
        break;
    };

    World::World* world = user->GetWorld();
    if (world)
    {
        World::EntityPainting* hangingEntity = new World::EntityPainting(x, y, z, orientation, item, world);
        if (!hangingEntity->IsDead())
        {
            world->AddEntity(hangingEntity);
        }
        else
        {
            delete hangingEntity;
            return World::ItemUseResult{false, false, 0};
        }
        return World::ItemUseResult{true, false, 1};
    }
    return World::ItemUseResult{false, false, 0};
}


} /* namespace Scripting */
