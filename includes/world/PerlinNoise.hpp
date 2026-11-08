#pragma once

class PerlinNoise {
private:
  // 再現可能なランダム値を作る
  static unsigned int  hash(int ix, int iz);
  static double  gradientDot(int ix, int iz, double dx, double dy);
  // 混ぜる割合を滑らかにする
  static double fade(double t);
  // 2つの値を混ぜる
  static double lerp(double a, double b, double t);

public:
  PerlinNoise();
  ~PerlinNoise();
  PerlinNoise(const PerlinNoise& other) = delete;
  PerlinNoise& operator=(const PerlinNoise& other) = delete;

  double  noise(double x, double z) const;
};
