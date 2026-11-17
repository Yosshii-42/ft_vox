#pragma once

#include "math/Vec3.hpp"

class SunMoon {
private:
#ifdef DEBUG_FAST_DAY_NIGHT
  static constexpr float  SUN_CYCLE_TIME = 20.0f;
#else
  static constexpr float  SUN_CYCLE_TIME = 120.0f;
#endif
  static constexpr float  PI = 3.14159265;
  static constexpr float  SUN_SPEED = 2.0 * PI / SUN_CYCLE_TIME;
  float sunAngle_ = 0.0f;
  
public:
  SunMoon() = default;
  ~SunMoon() = default;
  SunMoon(const SunMoon& other) = delete;
  SunMoon&  operator=(const SunMoon& other) = delete;

  void  update(float deltaTime);
  Vec3  getSunDirection() const;
  float getSunLightStrength(const Vec3& sunDir) const;
  float getAmbientStrength(const Vec3& sunDir) const;
  float getSkyLightStrength(const Vec3& sunDir) const;
};
