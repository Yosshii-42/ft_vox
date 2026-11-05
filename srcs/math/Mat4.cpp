#include "math/Mat4.hpp"

Mat4::Mat4() {
  for (int row = 0; row < 4; ++row)
  {
    for (int col = 0; col < 4; ++col)
    {
      if (row == col)
        this->m_[row][col] = 1.0f;
      else
        this->m_[row][col] = 0.0f;
    }
  }
}

Mat4::~Mat4() {}

Mat4::Mat4(const Mat4& other)
{
  for (int row = 0; row < 4; ++row)
  {
    for (int col = 0; col < 4; ++col)
    {
      this->m_[row][col] = other.m_[row][col];
    }
  }
}

Mat4& Mat4::operator=(const Mat4& other)
{
  if (this != &other)
  for (int row = 0; row < 4; ++row)
  {
    for (int col = 0; col < 4; ++col)
    {
      this->m_[row][col] = other.m_[row][col];
    }
  }

  return (*this);
}

// functions
const float*  Mat4::data() const
{
  return &this->m_[0][0];
}

// degtreeからradianを算出する
float Mat4::radians(float degrees)
{
  return degrees * M_PI / 180.0f;
}

Mat4 Mat4::scale(float x, float y, float z)
{
  Mat4 result;

  result.m_[0][0] = x;
  result.m_[1][1] = y;
  result.m_[2][2] = z;
  
  return result;
}

Mat4 Mat4::translate(float x, float y, float z)
{
  Mat4 result;

  result.m_[0][3] = x;
  result.m_[1][3] = y;
  result.m_[2][3] = z;

  return result;
}

Mat4  Mat4::rotate(float angle, const Vec3& inputAxis)
{
  Mat4  result;
  Vec3  axis = inputAxis.normalize();

  float x = axis.x;
  float y = axis.y;
  float z = axis.z;

  float c = std::cos(angle);
  float s = std::sin(angle);

  result.m_[0][0] = c + x * x * (1 - c);
  result.m_[0][1] = x * y * (1 - c) - z * s;
  result.m_[0][2] = x * z * (1 - c) + y * s;
  result.m_[0][3] = 0.0f;

  result.m_[1][0] = y * x * (1 - c) + z * s;
  result.m_[1][1] = c + y * y * (1 - c);
  result.m_[1][2] = y * z * (1 - c) - x * s;
  result.m_[1][3] = 0.0f;

  result.m_[2][0] = z * x * (1 - c) - y * s;
  result.m_[2][1] = z * y * (1 - c) + x * s;
  result.m_[2][2] = c + z * z * (1 - c);
  result.m_[2][3] = 0.0f;

  result.m_[3][0] = 0.0f;
  result.m_[3][1] = 0.0f;
  result.m_[3][2] = 0.0f;
  result.m_[3][3] = 1.0f;

  return result;
}

Mat4  Mat4::ortho(float left, float right, float bottom, float top, float near, float far)
{
  Mat4  result;

  result.m_[0][0] = 2.0f / (right - left);
  result.m_[0][3] = -(right + left) / (right - left);
  
  result.m_[1][1] = 2.0f / (top - bottom);
  result.m_[1][3] = -(top + bottom) / (top - bottom);

  result.m_[2][2] = -2.0f / (far - near);
  result.m_[2][3] = -(far + near) / (far - near);

  return result;
}

Mat4  Mat4::perspective(float fov, float aspect, float near, float far)
{
  float top = near * std::tan(fov / 2.0f);
  float bottom = -top;
  float right = top * aspect;
  float left = -right;

  return Mat4::frustum(left, right, bottom, top, near, far);
}

Mat4  Mat4::frustum(float left, float right, float bottom, float top, float near, float far)
{
  Mat4  result;

  result.m_[0][0] = 2.0f * near / (right - left);
  result.m_[0][2] = (right + left) / (right - left);

  result.m_[1][1] = 2.0f * near / (top - bottom);
  result.m_[1][2] = (top + bottom) / (top - bottom);

  result.m_[2][2] = -(far + near) / (far - near);
  result.m_[2][3] = -2.0f * far * near / (far - near);

  result.m_[3][2] = -1.0f;
  result.m_[3][3] = 0.0f;

  return result;
}

// operators
Mat4 Mat4::operator*(const Mat4& right) const
{
  Mat4 result;

  for (int row = 0; row < 4; ++row)
  {
    for (int col = 0; col < 4; ++col)
    {
      result.m_[row][col] = 0.0f;
      for (int k = 0; k < 4; ++k)
        result.m_[row][col] += this->m_[row][k] * right.m_[k][col];
    }
  }

  return result;
}

Vec4  Mat4::operator*(const Vec4& right) const
{
  Vec4  result;

  result.x = this->m_[0][0] * right.x +
             this->m_[0][1] * right.y +
             this->m_[0][2] * right.z +
             this->m_[0][3] * right.w;
  result.y = this->m_[1][0] * right.x +
             this->m_[1][1] * right.y +
             this->m_[1][2] * right.z +
             this->m_[1][3] * right.w;
  result.z = this->m_[2][0] * right.x +
             this->m_[2][1] * right.y +
             this->m_[2][2] * right.z +
             this->m_[2][3] * right.w;
  result.w = this->m_[3][0] * right.x +
             this->m_[3][1] * right.y +
             this->m_[3][2] * right.z +
             this->m_[3][3] * right.w;
  return result;
}

Mat4& Mat4::operator*=(const Mat4& right)
{
  *this = *this * right;
  return *this;
}

float Mat4::getM(int row, int col) const
{
  return (m_[row][col]);
}

// overload
std::ostream& operator<<(std::ostream& out, const Mat4& mat) {
  out << mat.m_[0][0] << " "
      << mat.m_[0][1] << " "
      << mat.m_[0][2] << " "
      << mat.m_[0][3] << std::endl;
  out << mat.m_[1][0] << " "
      << mat.m_[1][1] << " "
      << mat.m_[1][2] << " "
      << mat.m_[1][3] << std::endl;
  out << mat.m_[2][0] << " "
      << mat.m_[2][1] << " "
      << mat.m_[2][2] << " "
      << mat.m_[2][3] << std::endl;
  out << mat.m_[3][0] << " "
      << mat.m_[3][1] << " "
      << mat.m_[3][2] << " "
      << mat.m_[3][3];
  return out;
}