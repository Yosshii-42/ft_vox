#pragma once

#include "math/Vec3.hpp"
#include "math/Vec4.hpp"

#include <cmath>
#include <ostream>

class Mat4 {
private:
  float m_[4][4];

public:
  Mat4();
  ~Mat4();
  Mat4(const Mat4& other);
  Mat4& operator=(const Mat4& other);

  // functions
  const float*  data() const;
  static float  radians(float degrees);
  // 拡大縮小
  static Mat4   scale(float x, float y, float  z);
  // 平行移動
  static Mat4   translate(float x, float y, float z);
  // 回転移動
  static Mat4   rotate(float angle, const Vec3& inputAxis);
  // 正書図法投影
  static Mat4   ortho(float left, float right, float bottom, float top, float near, float far);
  // 透視投影
  static Mat4   perspective(float fov, float aspect, float near, float far);
  static Mat4   frustum(float left, float rgiht, float bottom, float top, float near, float far);

  // operators
  Mat4  operator*(const Mat4& right) const;
  Vec4  operator*(const Vec4& right) const;
  Mat4& operator*=(const Mat4& right);

  float getM(int row, int col) const;

  // overload
  friend std::ostream& operator<<(std::ostream& out, const Mat4& mat);
};

