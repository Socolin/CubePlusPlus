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

const int xOffsetsForSidesYZX[6] = {0, 0, 0, 0, -1, 1};
const int yOffsetsForSidesYZX[6] = {-1, 1, 0, 0, 0, 0};
const int zOffsetsForSidesYZX[6] = {0, 0, -1, 1, 0, 0};

#define FOR_EACH_SIDE_YZX(BlocX, BlocY, BlocZ, VarName)             \
        for (int side = 0; side < 6; side++)                        \
        {                                                           \
            int VarName##Y = BlocY + yOffsetsForSidesYZX[side];     \
            if (VarName##Y < 0 || VarName##Y > 255)                 \
                continue;                                           \
            int VarName##X = BlocX + xOffsetsForSidesYZX[side];     \
            int VarName##Z = BlocZ + zOffsetsForSidesYZX[side];     \

const int xOffsetsForSidesXYZ[6] = {-1, 1, 0, 0, 0, 0};
const int yOffsetsForSidesXYZ[6] = {0, 0, -1, 1, 0, 0};
const int zOffsetsForSidesXYZ[6] = {0, 0, 0, 0, -1, 1};

#define FOR_EACH_SIDE_XYZ(BlocX, BlocY, BlocZ, VarName)             \
        for (int side = 0; side < 6; side++)                        \
        {                                                           \
            int VarName##Y = BlocY + yOffsetsForSidesXYZ[side];     \
            if (VarName##Y < 0 || VarName##Y > 255)                 \
                continue;                                           \
            int VarName##X = BlocX + xOffsetsForSidesXYZ[side];     \
            int VarName##Z = BlocZ + zOffsetsForSidesXYZ[side];     \

const int xOffsetsForSidesYXZ[6] = {0, 0, -1, 1, 0, 0};
const int yOffsetsForSidesYXZ[6] = {-1, 1, 0, 0, 0, 0};
const int zOffsetsForSidesYXZ[6] = {0, 0, 0, 0, -1, 1};

#define FOR_EACH_SIDE_YXZ(BlocX, BlocY, BlocZ, VarName)             \
        for (int side = 0; side < 6; side++)                        \
        {                                                           \
            int VarName##Y = BlocY + yOffsetsForSidesYXZ[side];     \
            if (VarName##Y < 0 || VarName##Y > 255)                 \
                continue;                                           \
            int VarName##X = BlocX + xOffsetsForSidesYXZ[side];     \
            int VarName##Z = BlocZ + zOffsetsForSidesYXZ[side];     \

const int xOffsetsForSidesXZ[4] = {-1, 1, 0, 0};
const int zOffsetsForSidesXZ[4] = {0, 0, -1, 1};

#define FOR_EACH_SIDE_XZ(BlocX, BlocZ, VarName)                     \
        for (int side = 0; side < 4; side++)                        \
        {                                                           \
            int VarName##X = BlocX + xOffsetsForSidesXZ[side];      \
            int VarName##Z = BlocZ + zOffsetsForSidesXZ[side];      \

#define END_FOR_EACH_SIDE }

#define BLOCK_COUNT 256

#endif
