#ifndef BLOCK_CONSTANTS_H
#define BLOCK_CONSTANTS_H

enum BlockFace
{
    FACE_BOTTOM, // -Y
    FACE_TOP, // +Y
    FACE_NORTH, // -Z
    FACE_SOUTH, // +Z
    FACE_WEST, // -X
    FACE_EAST, // +X
    FACE_NONE = -1,
};

const int xOffsetsForSides[6] = {0, 0, 0, 0, -1, 1};
const int yOffsetsForSides[6] = {-1, 1, 0, 0, 0, 0};
const int zOffsetsForSides[6] = {0, 0, -1, 1, 0, 0};

#define BLOCK_COUNT 256

#endif
