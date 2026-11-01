#pragma once

#include "math/Mat4.hpp"
#include "math/Vec3.hpp"
#include "Common.hpp"

class Camera {
private:
  Vec3  position_;
  float yaw_;
  float pitch_;
  float moveSpeed_ = 5.0f;
  float rotationSpeed_ = Mat4::radians(90.0f);
  // float distance_;
  float fov_;
  float near_;
  float far_;

public:
  Camera();
  ~Camera();
  Camera(const Camera& other) = delete;
  Camera& operator=(const Camera& other) = delete;

  void  setPosition(const Vec3& position);

  void  moveForward(float deltaTime);
  void  moveBackward(float deltaTime);
  void  moveRight(float deltaTime);
  void  moveLeft(float deltaTime);
  void  moveUp(float deltaTime);
  void  moveDown(float deltaTime);

  void  turnLeft(float deltaTime);
  void  turnRight(float deltaTime);
  void  turnUp(float deltaTime);
  void  turnDown(float deltaTime);

  void  rotate(float drltaX, float deltaY);

  Mat4  getViewMatrix() const;
  Mat4  getProjectionMatrix(float aspectRatio) const;
};
