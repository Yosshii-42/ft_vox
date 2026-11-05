#include "math/Frustum.hpp"
#include "math/Plane.hpp"

Frustum::Frustum(const Mat4& view, const Mat4& projection)
{
  const Mat4 vp = projection * view;

  for (int row = 0; row < 3; ++row)
  {
    for (int fSide = 0; fSide < 2; ++fSide)
    {
      int sign = (fSide == 0) ? 1 : -1;
      // Left, Right, Bottom, Top, Near, Far の順
      Plane&  plane = planes_[row * 2 + fSide];

      plane.normal = Vec3(
        vp.getM(3, 0) + sign * vp.getM(row, 0),
        vp.getM(3, 1) + sign * vp.getM(row, 1),
        vp.getM(3, 2) + sign * vp.getM(row, 2)
      );
      plane.d = vp.getM(3, 3) + sign * vp.getM(row, 3);

    }
  }
}

Frustum::~Frustum() {}

// s：チャンク中心を平面の方程式に代入した値
// r：平面の法線方向へのAABBの投影半幅（法線未正規化）
bool  Frustum::intersectsAABB(const Vec3& min, const Vec3& max) const
{
  const Vec3  center = (min + max) * 0.5f;
  const Vec3  extents = (max - min) * 0.5f;

  for (const Plane& plane : planes_)
  {
    const float s = plane.normal.dot(center) + plane.d;
    const float r = std::abs(plane.normal.x) * extents.x +
                    std::abs(plane.normal.y) * extents.y +
                    std::abs(plane.normal.z) * extents.z;

    if (s + r < 0.0f)
      return (false);
  }
  return (true);
}
