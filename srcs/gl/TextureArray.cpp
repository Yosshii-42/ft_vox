#include "gl/TextureArray.hpp"

#include <stb/stb_image.h>

TextureArray::~TextureArray()
{
  if (textureID_ != 0)
    glDeleteTextures(1, &textureID_);
}

void  TextureArray::load(const std::vector<std::string>& paths)
{
  if (paths.empty())
    throw std::runtime_error("No texture paths provided");

  int width = 0;
  int height = 0;
  int channels = 0;

  unsigned char* pixels = stbi_load(
    paths[0].c_str(),
    &width,
    &height,
    &channels,
    STBI_rgb_alpha
  );

  if (pixels == nullptr)
      throw std::runtime_error("Failed to load texture:");

  stbi_image_free(pixels);

  glGenTextures(1, &textureID_);
  glBindTexture(GL_TEXTURE_2D_ARRAY, textureID_);

  // GPUに画像を格納する領域を確保する
  glTexImage3D(GL_TEXTURE_2D_ARRAY,
               0,
               GL_RGBA8,
               width, height,
               static_cast<GLsizei>(paths.size()),  // レイヤー数
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               nullptr
  );

  // CPU->GPU
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // 各画像を各レイヤーへ
  for (size_t i = 0; i < paths.size(); ++i)
  {
    int w = 0;
    int h = 0;
    int c = 0;

    unsigned char* pixels = stbi_load(paths[i].c_str(), &w, &h, &c, STBI_rgb_alpha);
    if (!pixels)
      throw std::runtime_error("Failed to load texture: " + paths[i]);
    if (w != width || h != height)
    {
      stbi_image_free(pixels);
      throw std::runtime_error("Texture size do not match");
    }

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                    0,
                    0, 0, static_cast<GLint>(i), // xoffset, yoffset, zoffset(書き込み先のレイヤー番号)
                    width, height, 1,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    pixels
    );

    stbi_image_free(pixels);
  }

  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void  TextureArray::bind() const
{
  glBindTexture(GL_TEXTURE_2D_ARRAY, textureID_);
}
