#include "gl/TextureArray.hpp"

#include <stb/stb_image.h>

TextureArray::~TextureArray()
{
  if (textureID_ != 0)
    glDeleteTextures(1, &textureID_);
}

void  TextureArray::load(const std::vector<std::string>& paths)
{
  (void)paths;

  int width = 0;
  int height = 0;
  int channels = 0;

  unsigned char* pixels = stbi_load(
    "textures/dirt.png",
    &width,
    &height,
    &channels,
    STBI_rgb_alpha
  );

  if (pixels == nullptr)
  {
      throw std::runtime_error(
          std::string("Failed to load texture: ")
          + stbi_failure_reason()
      );
  }

  glGenTextures(1, &textureID_);
  glBindTexture(GL_TEXTURE_2D_ARRAY, textureID_);

  // GPUに画像を格納する領域を確保する
  glTexImage3D(GL_TEXTURE_2D_ARRAY,
               0,
               GL_RGBA8,
               width, height,
               1, 0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               nullptr
  );

  // CPU->GPU
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                  0,
                  0, 0, 0,          // xoffset, yoffset, zoffset(書き込み先のレイヤー番号)
                  width, height, 1,
                  GL_RGBA,
                  GL_UNSIGNED_BYTE,
                  pixels
  );

  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

  stbi_image_free(pixels);
}

void  TextureArray::bind() const
{
  glBindTexture(GL_TEXTURE_2D_ARRAY, textureID_);
}
