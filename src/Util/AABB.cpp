#include "AABB.h"

namespace Util
{

AABB::AABB(float width, float height, float depht)
    : width(width)
    , height(height)
    , depth(depht)
    , x(0)
    , y(0)
    , z(0)
{
}

AABB::AABB(float x, float y, float z, float width, float height, float depht)
    : width(width)
    , height(height)
    , depth(depht)
    , x(x)
    , y(y)
    , z(z)
{
}

AABB::~AABB()
{
}

bool AABB::DetectCollision(const AABB& B) const
{
    if (x + width < B.x || y + height < B.y || z + depth < B.z || B.x + B.width < x || B.y + B.height < y || B.z + B.depth < z)
        return false;

    return true;
}

void AABB::SetPosition(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void AABB::SetPositionCenteredXZ(float x, float y, float z)
{
    this->x = x - width / 2;
    this->y = y;
    this->z = z - depth / 2;
}


float AABB::GetXOffsetWith(const AABB& B, float dx) const
{
    if (B.y + B.height > B.y && B.y < y + height)
    {
        if (B.z + B.depth > z && B.z < z + depth)
        {
            double distance;

            if (dx > 0.f && B.x + B.width <= x)
            {
                distance = x - (B.x + B.width);

                if (distance < dx)
                {
                    dx = distance;
                }
            }

            if (dx < 0.f && B.x >= x + width)
            {
                distance = (x + width) - B.x;

                if (distance > dx)
                {
                    dx = distance;
                }
            }
        }
    }
    return dx;
}

float AABB::GetYOffsetWith(const AABB& B, float dy) const
{
    if ((B.x + B.width) > x && B.x < x + width)
    {
        if ((B.z + B.depth) > z && B.z < z + depth)
        {
            double distance;

            if (dy > 0.f && (B.y + B.height) <= y)
            {
                distance = y - (B.y + B.height);

                if (distance < dy)
                {
                    dy = distance;
                }
            }

            if (dy < 0.f && B.y >= (y + height))
            {
                distance = (y + height) - B.y;

                if (distance > dy)
                {
                    dy = distance;
                }
            }
        }
    }
    return dy;
}

void AABB::Move(float dx, float dy, float dz)
{
    x += dx;
    y += dy;
    z += dz;
}

float AABB::GetZOffsetWith(const AABB& B, float dz) const
{
    if ((B.x + B.width) > x && B.x < x + width)
    {
        if ((B.y + B.height) > y && B.y < (y + height))
        {
            double distance;

            if (dz > 0.f && (B.z + B.depth) <= z)
            {
                distance = z - (B.z + B.depth);

                if (distance < dz)
                {
                    dz = distance;
                }
            }

            if (dz < 0.f && B.z >= z + depth)
            {
                distance = z + depth - B.z;

                if (distance > dz)
                {
                    dz = distance;
                }
            }
        }
    }
    return dz;
}





} /* namespace Inventory */
