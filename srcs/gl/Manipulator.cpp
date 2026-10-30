#include "gl/Manipulator.hpp"
#include "GLFW/glfw3.h"

Manipulator::Manipulator(const Vec3& center)
  : center_(center),
    rotation_(),
    position_(0.0f, 0.0f, 0.0f),
    scale_(1.0f, 1.0f, 1.0f),
    randomRotationAngle_(0.0f)
{}

Manipulator::~Manipulator() {}

void  Manipulator::rotateX(float angle)
{
  rotation_ = Mat4::rotate(angle, Vec3(1.0f, 0.0f, 0.0f)) * rotation_;
}

void  Manipulator::rotateY(float angle)
{
  rotation_ = Mat4::rotate(angle, Vec3(0.0f, 1.0f, 0.0f)) * rotation_;
}

void  Manipulator::rotateZ(float angle)
{
  rotation_ = Mat4::rotate(angle, Vec3(0.0f, 0.0f, 1.0f)) * rotation_;
}

void  Manipulator::rotateInstance(float angle)
{
  instanceAngle_ += angle;
}

void  Manipulator::rotateRandomAxis(float angle)
{
  randomRotationAngle_ += angle;
}

void  Manipulator::translateX(float trans)
{
  position_.x += trans;
}

void  Manipulator::translateY(float trans)
{
  position_.y += trans;
}

void  Manipulator::translateZ(float trans)
{
  position_.z += trans;
}

void  Manipulator::scale(float scale)
{
  scale_ *= scale;
}

// 頂点に center補正 → scale → rotate → translate の順で適用する
Mat4  Manipulator::getModelMatrix() const
{
  Mat4  model;

  // 最後に適用したい translate(移動) を最初に書く
  model *= Mat4::translate(position_.x, position_.y, position_.z);

  // rotate
  model *= rotation_;

  // 最初に適用したい　scale を最後に書く
  model *= Mat4::scale(scale_.x, scale_.y, scale_.z);

  // center補正
  model *= Mat4::translate(-center_.x, -center_.y, -center_.z);

  return (model);
}
