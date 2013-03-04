/*
 * Position.h
 *
 *  Created on: 24 déc. 2012
 *      Author: bertrand
 */

#ifndef POSITION_H_
#define POSITION_H_

#include <cmath>
namespace World
{

class Position
{
public:
    Position();
    Position(double x, double y, double z) :
        x(x), y(y), z(z)
    {
    }
    virtual ~Position();

    double GetDistanceSQ(const Position& otherPos)
    {
        double dx = x - otherPos.x;
        dx *= dx;
        double dy = y - otherPos.y;
        dy *= dy;
        double dz = z - otherPos.z;
        dz *= dz;
        return (dx + dy + dz);
    }

    double GetDistance(const Position& otherPos)
    {
        return sqrt(GetDistanceSQ(otherPos));
    }

    virtual void Relocate(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

public:
    double x;
    double y;
    double z;
};

} /* namespace Entity */
#endif /* POSITION_H_ */
