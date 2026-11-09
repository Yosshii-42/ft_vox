#include "world/PerlinNoise.hpp"
#include <cmath>

PerlinNoise::PerlinNoise() {}

PerlinNoise::~PerlinNoise() {}

// 再現可能なランダム値を作る
unsigned int  PerlinNoise::hash(int ix, int iz)
{
  unsigned int  h = static_cast<unsigned int>(ix) * 374761393u +
                    static_cast<unsigned int>(iz) * 668265263u;

  h = (h ^ (h >> 12)) * 1274126177u;
  return (h ^ (h >> 16));
}

double  PerlinNoise::gradientDot(int ix, int iz, double dx, double dz)
{
  switch (hash(ix, iz) % 4u)
  {
    case 0: return dx;
    case 1: return -dx;
    case 2: return dz;
    default: return -dz;
  }
}

// perlin noise = 6t^5 - 15t^4 + 10t^3
double  PerlinNoise::fade(double t)
{
  return (t * t * t * (t * (6.0 * t - 15.0) + 10.0));
}

double  PerlinNoise::lerp(double a, double b, double t)
{
  return (a + t * (b - a));
}

double  PerlinNoise::noise(double x, double z) const
{
  int x0 = static_cast<int>(std::floor(x));
  int z0 = static_cast<int>(std::floor(z));

  int x1 = x0 + 1;
  int z1 = z0 + 1;

  // 差分を求める
  double  dx = x - x0;
  double  dz = z - z0;

  // 4つの格子点の勾配ベクトルとの内積を求める
  double  n00 = gradientDot(x0, z0, dx,       dz);        // 左上
  double  n10 = gradientDot(x1, z0, dx - 1.0, dz);        // 右上
  double  n01 = gradientDot(x0, z1, dx,       dz - 1.0);  // 左下
  double  n11 = gradientDot(x1, z1, dx - 1.0, dz - 1.0);  // 右下

  // fadeで補間率を求める
  double  u = fade(dx); // x方向
  double  v = fade(dz); // z方向

  // x方向に補完する
  double  nx0 = lerp(n00, n10, u);
  double  nx1 = lerp(n01, n11, u);

  // z方向に補完する
  return  (lerp(nx0, nx1, v));
}
