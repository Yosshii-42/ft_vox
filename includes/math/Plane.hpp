#pragma once

#include <math/Vec3.hpp>

struct Plane {
  Vec3  normal;
  float d;

  float signedDistance(const Vec3& point) const
  {
    return (normal.dot(point) + d);
  }
};
