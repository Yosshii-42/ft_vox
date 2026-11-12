#include "math/Vec3.hpp"

Vec3::Vec3()
  : x(0.0f), y(0.0f), z(0.0f) {
}

Vec3::Vec3(float x, float y, float z)
  : x(x), y(y), z(z) {
}

// constexpr Vec3::(float x, float y, float z)
//   : x(x), y(y), z(z) {

//   }

Vec3::~Vec3() {}

// operators
Vec3  Vec3::operator+(const Vec3& right) const {
  Vec3  result;
  result.x = this->x + right.x;
  result.y = this->y + right.y;
  result.z = this->z + right.z;
  return result;
}

Vec3  Vec3::operator-(const Vec3& right) const {
  Vec3  result;
  result.x = this->x - right.x;
  result.y = this->y - right.y;
  result.z = this->z - right.z;
  return result;
}

Vec3  Vec3::operator*(float scalar) const {
  Vec3  result;
  result.x = this->x * scalar;
  result.y = this->y * scalar;
  result.z = this->z * scalar;
  return result;
}

Vec3  Vec3::operator/(float scalar) const {
  if (scalar == 0.0f)
    throw std::runtime_error("Vec3: division by zero.");
  Vec3  result;
  result.x = this->x / scalar;
  result.y = this->y / scalar;
  result.z = this->z / scalar;
  return result;
}

Vec3& Vec3::operator+=(const Vec3& right) {
  this->x += right.x;
  this->y += right.y;
  this->z += right.z;
  return *this;
}

Vec3& Vec3::operator-=(const Vec3& right) {
  this->x -= right.x;
  this->y -= right.y;
  this->z -= right.z;
  return *this;
}

Vec3& Vec3::operator*=(float scalar) {
  this->x *= scalar;
  this->y *= scalar;
  this->z *= scalar;
  return *this;
}

Vec3& Vec3::operator/=(float scalar) {
  if (scalar == 0.0f)
    throw std::runtime_error("Vec3: division by zero");
  this->x /= scalar;
  this->y /= scalar;
  this->z /= scalar;
  return *this;
}

bool  Vec3::operator==(const Vec3& right) const {
  return (this->x == right.x &&
          this->y == right.y &&
          this->z == right.z);
}

bool  Vec3::operator!=(const Vec3& right) const {
  return (this->x != right.x ||
          this->y != right.y ||
          this->z != right.z);
}

// functions
float Vec3::length() const {
  return (std::sqrt(
            this->x * this->x +
            this->y * this->y +
            this->z * this->z));
}

Vec3  Vec3::normalize() const {
  float length = this->length();
  if (length == 0.0f)
    throw std::runtime_error("Vec3: division by zero");
  return (*this/length);
}

float Vec3::dot(const Vec3& right) const {
  return (this->x * right.x +
          this->y * right.y +
          this->z * right.z);
}

Vec3  Vec3::cross(const Vec3& right) const {
  Vec3  result;
  result.x = this->y * right.z - this->z * right.y;
  result.y = this->z * right.x - this->x * right.z;
  result.z = this->x * right.y - this->y * right.x;
  return result;
}

// overload
std::ostream& operator<<(std::ostream& out, const Vec3& vec) {
  out << "("
      << vec.x << ", "
      << vec.y << ", "
      << vec.z
      << ")";
  return out;
}
