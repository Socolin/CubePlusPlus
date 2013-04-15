#include "BlockUtil.h"

#include "Block/BlockConstants.h"

namespace Util
{

bool UpdateXYZForSide(int side, int& x, i_height& y, int& z)
{
    switch (side)
    {
    case FACE_BOTTOM: // -Y
        if (y == 0)
            return false;
        y--;
        break;
    case FACE_TOP: // +Y
        if (y == 255)
            return false;
        y++;
        break;
    case FACE_NORTH: // -Z
        z--;
        break;
    case FACE_SOUTH: // +Z
        z++;
        break;
    case FACE_WEST: // -X
        x--;
        break;
    case FACE_EAST: // +X
        x++;
        break;
    case FACE_NONE:
        return false;
    };
    return true;
}

} /* namespace Util */
