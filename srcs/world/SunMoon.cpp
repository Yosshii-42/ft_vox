#include "world/SunMoon.hpp"
#include <algorithm>
#include <cmath>

void  SunMoon::update(float deltaTime)
{
  sunAngle_ += SUN_SPEED * deltaTime;

  if (sunAngle_ >= 2.0f * PI)
    sunAngle_ -= 2.0f * PI;
}

Vec3  SunMoon::getSunDirection() const
{
  return (Vec3(std::cos(sunAngle_), std::sin(sunAngle_), 0.0f));  
}

float SunMoon::getSunLightStrength(const Vec3& sunDir) const
{
  return (std::max(0.0f, sunDir.y * 0.25f));
}

float SunMoon::getAmbientStrength(const Vec3& sunDir) const
{
  return (0.1f + std::max(0.0f, sunDir.y) * 0.65f);
}

float SunMoon::getSkyLightStrength(const Vec3& sunDir) const
{
  return (std::max(0.0f, (sunDir.x + sunDir.y) * 0.25f));
}
