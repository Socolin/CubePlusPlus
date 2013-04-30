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


bool ItemPaintingScript::OnUseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, Inventory::ItemStack& item, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
    int orientation = -1;
    switch (face)
    {
    case FACE_BOTTOM: // -Y
    case FACE_TOP: // +Y
    case FACE_NONE:
        return false;
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

    World::World* world = user->getWorld();
    if (world)
    {
        World::EntityPainting* hangingEntity = new World::EntityPainting(x, y, z, orientation, item, world);
        if (!hangingEntity->isDead())
        {
            world->AddEntity(hangingEntity);
        }
        else
        {
            delete hangingEntity;
            return false;
        }
        return true;
    }
    return false;
}


} /* namespace Scripting */
