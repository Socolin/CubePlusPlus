#ifndef AABB_H_
#define AABB_H_

namespace Util
{

class AABB
{
public:
    AABB(float width, float height, float depht);
    AABB(float x, float y, float z, float width, float height, float depht);
    virtual ~AABB();

    bool DetectCollision(const AABB& B) const;
    void SetPosition(float x, float y, float z);
    void SetPositionCenteredXZ(float x, float y, float z);
    void MoveX(float dx) {x += dx;}
    void MoveY(float dy) {y += dy;}
    void MoveZ(float dz) {z += dz;}
    void Move(float dx, float dy, float dz);

    float GetXOffsetWith(const AABB& B, float dx) const;
    float GetYOffsetWith(const AABB& B, float dy) const;
    float GetZOffsetWith(const AABB& B, float dz) const;

    float width;
    float height;
    float depth;
    float x;
    float y;
    float z;
};

} /* namespace Inventory */
#endif /* AABB_H_ */
