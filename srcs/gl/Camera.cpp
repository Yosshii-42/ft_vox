#include "gl/Camera.hpp"

#include <algorithm>

Camera::Camera()
  : yaw_(0.0f),
    pitch_(0.0f),
    fov_(Common::CAMERA_FOV),
    near_(Common::CAMERA_NEAR),
    far_(Common::CAMERA_FAR)
{}

Camera::~Camera() {}


void  Camera::setPosition(const Vec3& position)
{
  position_ = position;
}

void  Camera::moveForward(float deltaTime)
{
  float distance = moveSpeed_ * deltaTime;

  position_.x -= std::sin(yaw_) * distance;
  position_.z -= std::cos(yaw_) * distance;
}

void  Camera::moveBackward(float deltaTime)
{
  float distance = moveSpeed_ * deltaTime;
  
  position_.x += std::sin(yaw_) * distance;
  position_.z += std::cos(yaw_) * distance;
}

void  Camera::moveRight(float deltaTime)
{
  float distance = moveSpeed_ * deltaTime;
  
  position_.x += std::cos(yaw_) * distance;
  position_.z -= std::sin(yaw_) * distance;
}

void  Camera::moveLeft(float deltaTime)
{
  float distance = moveSpeed_ * deltaTime;
  
  position_.x -= std::cos(yaw_) * distance;
  position_.z += std::sin(yaw_) * distance;
}

void  Camera::moveUp(float deltaTime)
{
  float distance = moveSpeed_ * deltaTime;
  
  position_.y += distance;
}

void  Camera::moveDown(float deltaTime)
{
  float distance = moveSpeed_ * deltaTime;
  
  position_.y -= distance;
}

void  Camera::turnLeft(float deltaTime)
{
  yaw_ += rotationSpeed_ * deltaTime;
}

void  Camera::turnRight(float deltaTime)
{
  yaw_ -= rotationSpeed_ * deltaTime;
}

void  Camera::turnUp(float deltaTime)
{
  pitch_ += rotationSpeed_ * deltaTime;
}

void  Camera::turnDown(float deltaTime)
{
  pitch_ -= rotationSpeed_ * deltaTime;
}

void  Camera::rotate(float deltaX, float deltaY)
{
  const float sensitivity = 0.002f;

  yaw_ -= deltaX * sensitivity;
  pitch_ -= deltaY * sensitivity;

  const float limit = Mat4::radians(89.0f);
  pitch_ = std::clamp(pitch_, -limit, limit);
}

Mat4  Camera::getViewMatrix() const
{
  Mat4  view;
  view *= Mat4::rotate(-pitch_, Vec3(1.0f, 0.0f, 0.0f));
  view *= Mat4::rotate(-yaw_, Vec3(0.0f, 1.0f, 0.0f));
  view *= Mat4::translate(-position_.x, -position_.y, -position_.z);
  return (view);
}

Mat4  Camera::getProjectionMatrix(float aspectRatio) const
{
  Mat4  projection;
  projection *= Mat4::perspective(Mat4::radians(fov_),
                                  aspectRatio,
                                  near_,
                                  far_);
  return (projection);
}
