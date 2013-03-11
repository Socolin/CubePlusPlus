#ifndef AABB_HXX_
#define AABB_HXX_

namespace Util
{

void AABB::SetWidthHeight(double width, double height)
{
    this->width = width;
    this->depth = width;
    this->height = height;
}

double AABB::getDepth() const
{
    return depth;
}

void AABB::setDepth(double depth)
{
    this->depth = depth;
}

double AABB::getHeight() const
{
    return height;
}

void AABB::setHeight(double height)
{
    this->height = height;
}

double AABB::getWidth() const
{
    return width;
}

void AABB::setWidth(double width)
{
    this->width = width;
}

double AABB::getX() const
{
    return x;
}

void AABB::setX(double x)
{
    this->x = x;
}

double AABB::getY() const
{
    return y;
}

void AABB::setY(double y)
{
    this->y = y;
}

double AABB::getZ() const
{
    return z;
}

void AABB::setZ(double z)
{
    this->z = z;
}

double AABB::getMaxX() const
{
    return x + width;
}
double AABB::getMaxY() const
{
    return y + height;
}
double AABB::getMaxZ() const
{
    return z + depth;
}
}
#endif
