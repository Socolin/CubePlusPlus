/*
 * Position.h
 *
 *  Created on: 24 déc. 2012
 *      Author: bertrand
 */

#ifndef POSITION_H_
#define POSITION_H_

#include <cmath>
namespace Entity
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

public:
	double x;
	double y;
	double z;
};

} /* namespace Entity */
#endif /* POSITION_H_ */
