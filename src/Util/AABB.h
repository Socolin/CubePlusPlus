#ifndef AABB_H_
#define AABB_H_

namespace Util
{

class AABB
{
public:
    AABB(double width, double height, double depht);
    AABB(double x, double y, double z, double width, double height, double depht);
    virtual ~AABB();

    bool DetectCollision(const AABB& B) const;
    void SetPosition(double x, double y, double z);
    void SetPositionCenteredXZ(double x, double y, double z);
    void MoveX(double dx) {x += dx;}
    void MoveY(double dy) {y += dy;}
    void MoveZ(double dz) {z += dz;}
    void Move(double dx, double dy, double dz);

    double GetXOffsetWith(const AABB& B, double dx) const;
    double GetYOffsetWith(const AABB& B, double dy) const;
    double GetZOffsetWith(const AABB& B, double dz) const;

    inline void SetWidthHeight(double width, double height);
    inline double getDepth() const;
    inline void setDepth(double depth);
    inline double getHeight() const;
    inline void setHeight(double height);
    inline double getWidth() const;
    inline void setWidth(double width);
    inline double getX() const;
    inline void setX(double x);
    inline double getY() const;
    inline void setY(double y);
    inline double getZ() const;
    inline void setZ(double z);

    inline double getMaxX() const;
    inline double getMaxY() const;
    inline double getMaxZ() const;

private:
    double width;
    double height;
    double depth;
    double x;
    double y;
    double z;
};

} /* namespace Util */

#include "AABB.hxx"

#endif /* AABB_H_ */
