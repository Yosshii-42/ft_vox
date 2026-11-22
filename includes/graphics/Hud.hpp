#pragma once

#include "gl/Shader.hpp"
#include <glad/glad.h>
#include <string>
#include <vector>

class Hud {
private:
  GLuint  textureID_ = 0;
  GLuint  VAO_ = 0;
  GLuint  VBO_ = 0;

  static constexpr int    FONT_ATLAS_COLS = 16;
  static constexpr int    FONT_ATLAS_ROWS = 16;
  static constexpr float  FONT_CELL_U = 1.0f / FONT_ATLAS_COLS;
  static constexpr float  FONT_CELL_V = 1.0f / FONT_ATLAS_ROWS;

public:
  Hud();
  ~Hud();
  Hud(const Hud& other) = delete;
  Hud& operator=(const Hud& other) = delete;

  void  load(const std::string& path);
  void  bind() const;
  void  setupMesh();
  void  draw(Shader& shader, const std::string& text,
             float x, float y, float width, float height) const;
};
