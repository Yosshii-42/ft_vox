#pragma once

#include "gl/Shader.hpp"
#include <glad/glad.h>
#include <string>
#include <vector>

class SkyBox {
private:
  GLuint  textureID_ = 0;
  GLuint  VAO_ = 0;
  GLuint  VBO_ = 0;

public:
  SkyBox();
  ~SkyBox();
  SkyBox(const SkyBox& other) = delete;
  SkyBox& operator=(const SkyBox& other) = delete;

  void  load(const std::vector<std::string>& path);
  void  bind() const;
  void  setupMesh();
  void  draw(Shader& shader) const;
};
