#include "graphics/Hud.hpp"
#include <stb/stb_image.h>
#include <stdexcept>

Hud::Hud()
{
  setupMesh();
}

Hud::~Hud() {
  if (textureID_ != 0)
    glDeleteTextures(1, &textureID_);
  glDeleteTextures(1, &VBO_);
  glDeleteVertexArrays(1, &VAO_);
}

void  Hud::load(const std::string& path)
{
  if (path.empty())
    throw std::runtime_error("NO texture path provided: hud");
  
  // 読み込み
  int width = 0;
  int height = 0;
  int channel = 0;

  unsigned char* pixels = stbi_load(
    path.c_str(), &width, &height, &channel, STBI_rgb_alpha);
  if (!pixels)
    throw std::runtime_error("Fail to load hud texture");

  // テクスチャを生成しバインド
  glGenTextures(1, &textureID_);
  glBindTexture(GL_TEXTURE_2D, textureID_);

  // GPUに画像を転送
  glTexImage2D(GL_TEXTURE_2D, 0,
              GL_RGBA8,  // 内部フォーマット
              width,
              height,
              0,
              GL_RGBA,
              GL_UNSIGNED_BYTE,
              pixels
              );
  stbi_image_free(pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   // 拡大時
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);   // 縮小時
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // U
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // V

  glBindTexture(GL_TEXTURE_2D, 0);
}

void  Hud::bind() const
{
  glBindTexture(GL_TEXTURE_2D, textureID_);
}

void  Hud::setupMesh()
{
  if (VAO_ == 0)
    glGenVertexArrays(1, &VAO_);
  if (VBO_ == 0)
    glGenBuffers(1, &VBO_);

  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, VAO_);

  glBufferData(GL_ARRAY_BUFFER,
               sizeof(float) * 4 * 6,
               nullptr,
               GL_DYNAMIC_DRAW
  );

  if (VAO_)
  {
    // position location = 0
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);
    // UV location = 1
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 4, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
}

void  Hud::draw(Shader& shader, const std::string& text,
                float x, float y, float width, float height) const
{
  shader.use();
  bind();

  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);

  float startX = x;
  for (char ch: text)
  {
    if (ch == '\n')
    {
      x = startX;
      y += height;
      continue;
    }
    
    int   col = ch % FONT_ATLAS_COLS;
    int   row = ch / FONT_ATLAS_COLS;
    float u0 = col * FONT_CELL_U;
    float v0 = row * FONT_CELL_V;
    float u1 = (col + 1) * FONT_CELL_U;
    float v1 = (row + 1) * FONT_CELL_V;

    float vertices[6][4] =
    {
      {x,         y,          u0, v0},  // A
      {x,         y + height, u0, v1},  // B
      {x + width, y + height, u1, v1},  // C

      {x + width, y + height, u1, v1},  // C
      {x + width, y         , u1, v0},  // D
      {x,         y,          u0, v0}  // A
    };

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    x += width;
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
