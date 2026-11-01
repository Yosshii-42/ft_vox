#pragma once

#include <glad/glad.h>
#include "gl/Shader.hpp"

class Crosshair {
private:
  GLuint  vao_;
  GLuint  vbo_;

public:
  Crosshair();
  ~Crosshair();

  Crosshair(const Crosshair& other) = delete;
  Crosshair& operator=(const Crosshair& other) = delete;

  void  draw(Shader& shader, float aspectRatio) const;
};