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


#define BLOCK_COUNT 256

#endif
