#pragma once

#include "math/Plane.hpp"
#include "math/Mat4.hpp"

class Frustum {
private:
  Plane planes_[6];

public:
  Frustum(const Mat4& view, const Mat4& projection);
  ~Frustum();

  bool  intersectsAABB(const Vec3& min, const Vec3& max) const;
};
