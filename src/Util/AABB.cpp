#include "AABB.h"
#include <iostream>

namespace Util
{

AABB::AABB(double width, double height, double depht)
    : width(width)
    , height(height)
    , depth(depht)
    , x(0)
    , y(0)
    , z(0)
{
}

AABB::AABB(double x, double y, double z, double width, double height, double depht)
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

void AABB::SetPosition(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void AABB::SetPositionCenteredXZ(double x, double y, double z)
{
    this->x = x - width / 2;
    this->y = y;
    this->z = z - depth / 2;
}
void AABB::SetPositionCenteredXYZ(double x, double y, double z)
{
    this->x = x - width / 2;
    this->y = y - height / 2;
    this->z = z - depth / 2;
}


double AABB::GetXOffsetWith(const AABB& B, double dx) const
{
    if ((B.y + B.height) > y && B.y < (y + height))
    {
        if ((B.z + B.depth) > z && B.z < (z + depth))
        {
            double distance;

            if (dx > 0.0 && (B.x + B.width) <= x)
            {
                distance = x - (B.x + B.width);

                if (distance < dx)
                {
                    dx = distance;
                }
            }

            if (dx < 0.0 && B.x >= (x + width))
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

double AABB::GetYOffsetWith(const AABB& B, double dy) const
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

void AABB::Move(double dx, double dy, double dz)
{
    x += dx;
    y += dy;
    z += dz;
}

double AABB::GetZOffsetWith(const AABB& B, double dz) const
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
