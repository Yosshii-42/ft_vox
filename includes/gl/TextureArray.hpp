#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

class TextureArray {
private:
  GLuint  textureID_ = 0;

public:
  TextureArray() = default;
  ~TextureArray();
  TextureArray(const TextureArray& other) = delete;
  TextureArray& operator=(const TextureArray& other) = delete;

  void  load(const std::vector<std::string>& paths);
  void  bind() const;
};
