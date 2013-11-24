#include "ItemFrameScript.h"

#include "Block/BlockConstants.h"
#include "Entity/Object/Hanging/EntityHangingFrame.h"
#include "Entity/EntityPlayer.h"
#include "Util/AssertUtil.h"
#include "World/World.h"

namespace Scripting
{

ItemFrameScript::ItemFrameScript()
    : ItemScript("item_frame")
{
}

ItemFrameScript::~ItemFrameScript()
{
}

ItemScript* ItemFrameScript::Copy()
{
    return new ItemFrameScript(*this);
}

World::ItemUseResult ItemFrameScript::OnUseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, char /*cursorPositionX*/, char /*cursorPositionY*/, char /*cursorPositionZ*/) const
{
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
        World::EntityHangingFrame* hangingEntity = new World::EntityHangingFrame(x, y, z, orientation);
        world->AddEntity(hangingEntity);
        return World::ItemUseResult{true, false, 1};
    }
    return World::ItemUseResult{false, false, 0};
}

} /* namespace Scripting */
